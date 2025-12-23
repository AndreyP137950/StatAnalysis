#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <iomanip>

#include "Source.hpp"

using namespace std;


// Структура выборки
struct Sample { vector<double> x; vector<int> r; int n = 0; } S;

static Sample read_input_normal(const string& tag)
{
    ifstream in("Inp/" + tag + ".inp");
    if (!in) { throw runtime_error("Не найден файл Inp/" + tag + ".inp"); }

    string s; Sample S; int N; double beta, eps;
    in >> s >> N; S.n = N;
    in >> s >> beta;
    in >> s >> eps;
    in >> s; S.x.resize(N); for (int i = 0; i < N; i++) in >> S.x[i];
    in >> s; S.r.resize(N); for (int i = 0; i < N; i++) in >> S.r[i];
    in >> s; int kp; in >> kp;
    vector<double> P(kp); in >> s; for (int i = 0; i < kp; i++) in >> P[i];
    ofstream tmp("Out/" + tag + ".probs"); for (double p : P) tmp << std::setprecision(17) << p << "\n";
    return S;
}



int main() {
    const string TAG = "MLE_Normal";
    auto S = read_input_normal(TAG);

    // Начальные по наблюдаемым (r=0)
    vector<double> obs;
    obs.reserve(S.n);
    for (int i = 0; i < S.n; i++) if (S.r[i] == 0) obs.push_back(S.x[i]);

    double a0 = 0.0, s0 = 1.0;
    if (!obs.empty()) {
        a0 = accumulate(obs.begin(), obs.end(), 0.0) / double(obs.size());
        double q = 0.0; for (double v : obs) q += (v - a0) * (v - a0);
        s0 = (obs.size() > 1) ? std::sqrt(q / double(obs.size() - 1)) : 1.0;
    }
    else {
        vector<double> xs = S.x; sort(xs.begin(), xs.end());
        a0 = xs[xs.size() / 2];
        s0 = std::max(1e-3, (xs.back() - xs.front()) / 6.0);
    }

    vector<double> theta{ a0, std::max(1e-6,s0) };
    bool censored = ((int)obs.size() != S.n);

    if (censored) {
        G_X = &S.x; G_R = &S.r;
        neldermead(theta, 1e-10, &normal_score_fn);
    }
    const double a_hat = theta[0], s_hat = theta[1];

    // Эмпирика (К–М)
    EmpiricalKM emp = kaplan_meier_Itype(S.x, S.r);
    vector<double> X_emp = emp.x_sorted, Y_emp;
    Y_emp.reserve(emp.F_emp.size());
    for (double F : emp.F_emp) Y_emp.push_back(5.0 + norm_ppf(F));

    // уровни p
    vector<double> P;
    {
        ifstream pin("Out/" + TAG + ".probs");
        for (string line; getline(pin, line);) if (!line.empty()) P.push_back(stod(line));
    }

    // ковариация оценок и интервалы квантилей
    auto V = cov_normal_asymp(S.x, S.r, a_hat, s_hat);
    const double beta = 0.95;
    const double z_beta = norm_ppf((1.0 + beta) / 2.0);

    vector<double> Xlow, Xest, Xupp, Yline;
    Xlow.reserve(P.size()); Xest.reserve(P.size()); Xupp.reserve(P.size()); Yline.reserve(P.size());

    for (double p : P) {
        const double z = norm_ppf(p);
        const double xp = a_hat + z * s_hat;
        const double var_xp = V[0][0] + 2.0 * V[0][1] * z + V[1][1] * z * z;
        const double sd_xp = std::sqrt(std::max(0.0, var_xp / double(S.n)));

        Xest.push_back(xp);
        Xlow.push_back(xp - z_beta * sd_xp);
        Xupp.push_back(xp + z_beta * sd_xp);
        Yline.push_back(5.0 + z);
    }

    const int nc = 4;
    vector<int> m = { (int)X_emp.size(), (int)P.size(), (int)P.size(), (int)P.size() };

    ofstream xo("Out/" + TAG + ".xout");
    xo << nc << "\n";
    for (int i = 0; i < nc; i++) xo << m[i] << (i + 1 < nc ? " " : "");
    xo << "\n";
    for (double v : X_emp) xo << std::setprecision(17) << v << " "; xo << "\n";
    for (double v : Xlow) xo << std::setprecision(17) << v << " "; xo << "\n";
    for (double v : Xest) xo << std::setprecision(17) << v << " "; xo << "\n";
    for (double v : Xupp) xo << std::setprecision(17) << v << " "; xo << "\n";
    for (double v : Y_emp) xo << std::setprecision(17) << v << " "; xo << "\n";
    for (double v : Yline) xo << std::setprecision(17) << v << " "; xo << "\n";
    for (double v : Yline) xo << std::setprecision(17) << v << " "; xo << "\n";
    for (double v : Yline) xo << std::setprecision(17) << v << " "; xo << "\n";
    xo.close();

    ofstream out("Out/" + TAG + ".out");
    out << "Method:" << TAG << "\n";
    out << "n=" << S.n << "\n";
    out << "X\n"; for (double v : S.x) out << v << " , "; out << "\n";
    out << "R\n"; for (int v : S.r) out << v << " , "; out << "\n";
    out << fixed << setprecision(12);
    out << "a_hat=" << a_hat << "\n";
    out << "sigma_hat=" << s_hat << "\n";
    out << "Cov[a,s]:\n";
    out << V[0][0] << " " << V[0][1] << "\n";
    out << V[1][0] << " " << V[1][1] << "\n";
    out << "P\n"; for (double p : P) out << p << " ; "; out << "\n";
    out << "Xp_low\n"; for (double v : Xlow) out << v << " ; "; out << "\n";
    out << "Xp\n"; for (double v : Xest) out << v << " ; "; out << "\n";
    out << "Xp_up\n"; for (double v : Xupp) out << v << " ; "; out << "\n";
    out.close();

    return 0;
}
