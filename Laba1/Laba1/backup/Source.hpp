#include <vector>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <boost/math/distributions/normal.hpp>

using namespace std;

vector<vector<double>> cov_normal_asymp(vector <double>& x, vector <int>& r, double a, double s);

// --- Глобальные ссылки для обёртки цели NM ---
static const std::vector<double>* G_X = nullptr;
static const std::vector<int>* G_R = nullptr;

struct EmpiricalKM {
    vector<double> x_sorted;   // только наблюдаемые (r=0)
    vector<double> F_emp;      // соответствующие F (К–М)
};

// Простая К–М для правой цензуры I-типа.
// Вход: x — значения; r — флаги (0 — наблюдение, 1 — цензура).
inline EmpiricalKM kaplan_meier_Itype(const vector<double>& x,
    const vector<int>& r)
{
    const int n = static_cast<int>(x.size());
    vector<int> idx(n);
    for (int i = 0; i < n; ++i) idx[i] = i;

    sort(idx.begin(), idx.end(),
        [&](int i, int j) { return x[i] < x[j]; });

    EmpiricalKM res;
    res.x_sorted.reserve(n);
    res.F_emp.reserve(n);

    double S = 1.0;   // выживаемость
    int at_risk = n;  // под риском

    for (int t = 0; t < n; ++t) {
        int i = idx[t];
        if (r[i] == 0) {
            // событие: d=1, S *= (at_risk - d) / at_risk
            S *= double(at_risk - 1) / double(at_risk);
            const double F = 1.0 - S;

            res.x_sorted.push_back(x[i]);
            res.F_emp.push_back(F);
        }
        // убывает под-риском и при событии, и при цензуре
        at_risk -= 1;
    }
    return res;
}

// 1 / sqrt(2*pi) — числовое значение, чтобы не зависеть от M_PI
// (корректно для double; хватает далеко за точность double)
static const double INV_SQRT_2PI = 0.39894228040143267793994605993438;

inline double norm_pdf(double z) {
    return INV_SQRT_2PI * exp(-0.5 * z * z);
}

inline double norm_cdf(double z) {
    boost::math::normal_distribution<> N(0.0, 1.0);
    return cdf(N, z);
}

inline double norm_ppf(double p) {
    if (p <= 0.0) return -INFINITY;
    if (p >= 1.0) return  INFINITY;
    boost::math::normal_distribution<> N(0.0, 1.0);
    return quantile(N, p);
}


inline int neldermead(vector<double>& x0,
    double eps,
    double(*func)(vector<double>))
{
    const int n = static_cast<int>(x0.size());
    const int maxit = 200 * (n > 0 ? n : 1);

    const double rho = 1.0, chi = 2.0, psi = 0.5, sigma = 0.5;
    const double nonz = 0.05, z = 0.00025;

    vector<vector<double>> S(n + 1, x0);
    for (int k = 0; k < n; ++k) {
        vector<double> y = x0;
        y[k] = (y[k] != 0.0) ? (1.0 + nonz) * y[k] : z;
        S[k + 1] = y;
    }

    auto op = [&](const vector<double>& A,
        const vector<double>& B,
        double t)
        {
            vector<double> r(n);
            for (int i = 0; i < n; ++i) r[i] = A[i] + t * (B[i] - A[i]);
            return r;
        };

    int it = 0;
    while (it++ < maxit) {
        // сортировка по значению функции (вызовы func K-раз — это ожидаемо)
        sort(S.begin(), S.end(),
            [&](const vector<double>& a, const vector<double>& b)
            { return func(a) < func(b); });

        // среднее и дисперсия значений цели на симплексе
        double m = 0.0;
        for (const auto& s : S) m += func(s);
        m /= (n + 1);

        double v = 0.0;
        for (const auto& s : S) {
            double fs = func(s);
            v += (fs - m) * (fs - m);
        }
        if (sqrt(v / (n + 1)) < eps) break;

        // центр тяжести без худшей вершины
        vector<double> c(n, 0.0);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                c[j] += S[i][j];
        for (double& cj : c) cj /= n;

        // отражение
        vector<double> xr = op(c, S[n], -rho);
        double fr = func(xr);

        if (fr < func(S[0])) {
            // расширение
            vector<double> xe = op(c, xr, chi);
            S[n] = (func(xe) < fr) ? xe : xr;
        }
        else if (fr < func(S[n - 1])) {
            // принимаем отражение
            S[n] = xr;
        }
        else {
            // сжатие
            vector<double> xc =
                (fr < func(S[n])) ? op(c, xr, psi) : op(c, S[n], -psi);
            if (func(xc) <= min(fr, func(S[n]))) {
                S[n] = xc;
            }
            else {
                // усадка к лучшей вершине
                for (int i = 1; i <= n; ++i)
                    for (int j = 0; j < n; ++j)
                        S[i][j] = S[0][j] + sigma * (S[i][j] - S[0][j]);
            }
        }
    }

    x0 = S[0];
    return it;
}

// --- «скор»-цель ММП ---
static double normal_score_fn(vector<double> theta)
{
    const vector<double>& x = *G_X;
    const vector<int>& r = *G_R;

    const double a = theta[0];
    const double s = theta[1];
    if (!(s > 0.0)) return 1e12;

    double S1 = 0.0, S2 = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        const double z = (x[i] - a) / s;
        const double p = norm_cdf(z);
        const double d = norm_pdf(z);
        const double psi = d / (1.0 - p);

        S1 += (1 - r[i]) * (x[i] - a) + r[i] * s * psi;
        S2 += (1 - r[i]) * ((x[i] - a) * (x[i] - a) - s * s)
            + r[i] * s * s * (psi * ((x[i] - a) / s) - 1.0);
    }
    S1 /= double(x.size());
    S2 /= double(x.size());
    const double val = S1 * S1 + S2 * S2;

    if (!isfinite(val)) return 1e12;
    return val;
}

// Ковариационная матрица оценок  - ВОЗВРАЩАЕМ ВЕКТОР 2x2
vector<vector<double>> cov_normal_asymp(vector <double>& x, vector <int> & r, double a, double s)
{
    double J11 = 0.0, J12 = 0.0, J22 = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        double z = (x[i] - a) / s;
        double p = norm_cdf(z), d = norm_pdf(z);
        double psi = d / (1.0 - p);

        J11 += (1 - r[i]) * (1.0) + r[i] * (psi * (psi - z));
        J12 += r[i] * psi * (z * (psi - z) - 1.0);
        J22 += (1 - r[i]) * (2.0) + r[i] * (z * z * (psi * (psi - z)) - 2.0 * psi * z + 1.0);
    }
    const double n = double(x.size());
    J11 /= n; J12 /= n; J22 /= n;

    double det = J11 * J22 - J12 * J12;
    if (!(det > 0.0)) det = 1e-12;

    // V = inv(J) для 2x2
    vector<vector<double>> V(2, vector<double>(2, 0.0));
    V[0][0] = J22 / det;  V[0][1] = -J12 / det;
    V[1][0] = -J12 / det;  V[1][1] = J11 / det;
    return V;
}

// ММП Вейбулла (двухпараметр., x0=0) — без изменений
static pair<double, double> weibull_mle_2par(const vector<double>& x, const vector<int>& r)
{
    vector<double> obs; obs.reserve(x.size());
    for (size_t i = 0; i < x.size(); ++i) if (r[i] == 0 && x[i] > 0.0) obs.push_back(x[i]);
    if (obs.size() < 2) throw runtime_error("Недостаточно наблюдений для ММП Вейбулла");

    vector<double> lx(obs.size());
    for (size_t i = 0; i < obs.size(); ++i) lx[i] = log(obs[i]);
    double m = accumulate(lx.begin(), lx.end(), 0.0) / double(lx.size());
    double s2 = 0.0; for (double v : lx) s2 += (v - m) * (v - m); s2 /= double(lx.size() - 1);
    double b = (s2 > 1e-12) ? 1.2 / sqrt(s2) : 2.0;
    if (b < 0.2)  b = 0.2;
    if (b > 20.0) b = 20.0;

    auto f = [&](double bb) {
        double A = 0.0, B = 0.0;
        for (double xi : obs) { double xb = pow(xi, bb); A += xb * log(xi); B += xb; }
        double lhs = 0.0; for (double xi : obs) lhs += log(xi);
        return lhs - A / B;
        };
    auto fp = [&](double bb) {
        double A = 0.0, B = 0.0, C = 0.0;
        for (double xi : obs) {
            double l = log(xi), xb = pow(xi, bb);
            A += xb * l; B += xb; C += xb * l * l;
        }
        double Aprime = C, Bprime = A;
        return -(Aprime * B - A * Bprime) / (B * B);
        };

    for (int it = 0; it < 50; ++it) {
        double val = f(b), der = fp(b);
        if (fabs(der) < 1e-12) break;
        double step = val / der;
        b -= step;
        if (b < 0.05) b = 0.05;
        if (fabs(step) < 1e-10) break;
    }
    double sum_xb = 0.0; for (double xi : obs) sum_xb += pow(xi, b);
    double c = pow(sum_xb / obs.size(), 1.0 / b);
    return { c,b };
}