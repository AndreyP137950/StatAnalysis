// mle_weibull.cpp — устойчивый ввод (без <bits>, без std::array/clamр)

#include <limits>   // std::numeric_limits
#include <cctype>   // std::tolower
#include <cstdlib>  // std::strtod
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

struct Sample { vector<double> x; vector<int> r; int n; };

static bool is_label(const std::string& t) {
    std::string s = t;
    for (char& c : s) c = (char)std::tolower((unsigned char)c);
    return s == "r" || s == "kp" || s == "p" || s == "beta" || s == "eps" || s == "n" || s == "x";
}

static bool try_parse_double_loose(const std::string& tok, double& out) {
    // принимает и запятые, и точку; возвращает false, если это не число
    std::string t = tok;
    for (char& c : t) if (c == ',') c = '.';
    char* endp = nullptr;
    const double v = std::strtod(t.c_str(), &endp);
    if (endp && *endp == '\0') { out = v; return std::isfinite(out); }
    return false;
}

// прочитать ровно need чисел после текущей позиции, пропуская мусор,
// но останавливаясь с ошибкой на "реальной" метке секции.
static std::vector<double> read_numbers_loose(std::istream& in, int need, const char* ctx)
{
    std::vector<double> a; a.reserve(need);
    std::string tok;
    while ((int)a.size() < need && (in >> tok)) {
        // если это "настоящая" метка — значит в файле не хватает чисел для секции ctx
        if (is_label(tok))
            throw std::runtime_error(std::string("Недостаточно чисел в секции '") + ctx + "': встретили метку '" + tok + "'");
        double v;
        if (try_parse_double_loose(tok, v)) {
            a.push_back(v);
        }
        else {
            // мусорный токен — просто пропускаем
            continue;
        }
    }
    if ((int)a.size() < need)
        throw std::runtime_error(std::string("Недостаточно чисел в секции '") + ctx + "' (ожидалось " + std::to_string(need) + ")");
    return a;
}



// Читаем ровно N индикаторов 0/1:
//  - токены близкие к 0 → 0; близкие к 1 → 1;
//  - всё остальное игнорируем как мусор;
//  - если встретили "настоящую" метку раньше, чем набрали N, выдаём ошибку.
static std::vector<int> read_R_indicators(std::istream& in, int N) {
    std::vector<int> R; R.reserve(N);
    std::string tok;
    while ((int)R.size() < N && (in >> tok)) {
        if (is_label(tok)) {
            if ((int)R.size() < N)
                throw std::runtime_error("Недостаточно значений в секции 'R' — встретили метку '" + tok + "'");
            break;
        }
        double v;
        if (!try_parse_double_loose(tok, v)) continue; // мусор — пропустим
        if (v <= 0.25)       R.push_back(0);
        else if (v >= 0.75 && v <= 1.25) R.push_back(1);
        else {
            // явный мусор — просто пропустим
            continue;
        }
    }
    if ((int)R.size() < N)
        throw std::runtime_error("Недостаточно значений в секции 'R' (ожидалось " + std::to_string(N) + ")");
    return R;
}


// --- утилита: прочитать токен как double, заменяя запятую на точку
static double parse_double_token(string tok, const string& ctx) {
    for (char& c : tok) if (c == ',') c = '.';
    try {
        size_t p = 0;
        double v = stod(tok, &p);
        if (p != tok.size()) throw runtime_error("junk");
        if (!std::isfinite(v)) throw runtime_error("inf/nan");
        return v;
    }
    catch (...) {
        throw runtime_error("Ошибка чтения числа (" + ctx + "): '" + tok + "'");
    }
}

static int parse_int_token(string tok, const string& ctx) {
    for (char& c : tok) if (c == ',') c = '.';
    try {
        size_t p = 0;
        long long v = stoll(tok, &p);
        if (p != tok.size()) throw runtime_error("junk");
        if (v < std::numeric_limits<int>::min() || v > std::numeric_limits<int>::max())
            throw runtime_error("range");
        return static_cast<int>(v);
    }
    catch (...) {
        throw runtime_error("Ошибка чтения целого (" + ctx + "): '" + tok + "'");
    }
}

// --- чтение Inp/<tag>.inp 
static Sample read_input_weibull(const string& tag)
{
    const string path = "Inp/" + tag + ".inp";
    ifstream in(path);
    if (!in) throw runtime_error("Не найден файл " + path);

    string s;
    Sample S{};
    int N = 0; double beta = 0.0, eps = 0.0;

    // n <N>
    if (!(in >> s)) throw runtime_error("Ожидался маркер 'n'");
    if (!(in >> N)) throw runtime_error("Ошибка чтения N");
    if (N <= 1 || N > 1000000) throw runtime_error("Некорректный N (2..1e6)");

    // beta <beta>
    if (!(in >> s)) throw runtime_error("Ожидался маркер 'beta'");
    {
        string tok; if (!(in >> tok)) throw runtime_error("Ошибка чтения beta");
        beta = parse_double_token(tok, "beta");
    }

    // eps <eps>
    if (!(in >> s)) throw runtime_error("Ожидался маркер 'eps'");
    {
        string tok; if (!(in >> tok)) throw runtime_error("Ошибка чтения eps");
        eps = parse_double_token(tok, "eps"); (void)eps;
    }

    // X <N чисел>  — устойчивый ввод
    if (!(in >> s)) throw runtime_error("Ожидался маркер 'X'");
    {
        auto xs = read_numbers_loose(in, N, "X");
        S.x = std::move(xs);
        for (int i = 0; i < N; ++i) {
            if (!(S.x[i] >= 0.0))
                throw runtime_error("X[" + to_string(i) + "] отрицательно");
        }
    }

    // R <N 0/1> 
    if (!(in >> s)) throw runtime_error("Ожидался маркер 'R'");
    {
        auto rv = read_R_indicators(in, N);
        S.r = std::move(rv);
    }



    // kp <int>
    if (!(in >> s)) throw runtime_error("Ожидался маркер 'kp'");
    int kp = 0;
    {
        string tok; if (!(in >> tok)) throw runtime_error("Ошибка чтения kp");
        kp = parse_int_token(tok, "kp");
    }
    if (kp <= 0 || kp > 10000) throw runtime_error("Некорректный kp (1..10000)");

    // P <kp чисел в [0;1]>
    if (!(in >> s)) throw runtime_error("Ожидался маркер 'P'");
    vector<double> P; P.reserve(kp);
    for (int i = 0; i < kp; ++i) {
        string tok; if (!(in >> tok)) throw runtime_error("Недостаточно значений P");
        double p = parse_double_token(tok, "P[" + to_string(i) + "]");
        if (!(p > 0.0 && p < 1.0)) throw runtime_error("P[" + to_string(i) + "] не в (0,1)");
        P.push_back(p);
    }

    // сохраняем уровни — как было
    ofstream tmp("Out/" + tag + ".probs");
    if (!tmp) throw runtime_error("Не удалось открыть Out/" + tag + ".probs для записи");
    tmp << setprecision(17);
    for (double p : P) tmp << p << "\n";

    S.n = N;
    (void)beta; // пока не используем
    return S;
}



// Грубая ковариация (2x2 через vector)
static std::vector<std::vector<double>> cov_weibull_asymp(const vector<double>& x,
    const vector<int>& r,
    double c, double b)
{
    vector<double> obs; obs.reserve(x.size());
    for (size_t i = 0; i < x.size(); ++i) if (r[i] == 0 && x[i] > 0.0) obs.push_back(x[i]);
    const double n = double(x.size());

    double Jpp = 0.0, Jpq = 0.0, Jqq = 0.0;
    for (double xi : obs) {
        double p = log(c);
        double q = 1.0 / b;
        double z = (log(xi) - p) / q;
        Jpp += 1.0;
        Jqq += (1.0 + std::fabs(z));
        Jpq += 0.1 * z;
    }
    Jpp /= n; Jpq /= n; Jqq /= n;

    double det = Jpp * Jqq - Jpq * Jpq; if (!(det > 0.0)) det = 1e-12;
    std::vector<std::vector<double>> V(2, std::vector<double>(2, 0.0));
    V[0][0] = Jqq / det;  V[0][1] = -Jpq / det;
    V[1][0] = -Jpq / det;  V[1][1] = Jpp / det;
    return V;
}

// Оценки b,c по линейной регрессии (фолбэк, если ММП неудачен)
static std::pair<double, double> weibull_regression_fallback(const std::vector<double>& x,
    const std::vector<int>& r)
{
    // эмпирика
    auto emp = kaplan_meier_Itype(x, r);

    std::vector<double> X, Y;
    X.reserve(emp.x_sorted.size());
    Y.reserve(emp.x_sorted.size());

    // y = ln ln(1/(1-F)), x = ln(X), берём только 0 < F < 1
    for (size_t i = 0; i < emp.x_sorted.size(); ++i) {
        double F = emp.F_emp[i];
        if (F > 1e-6 && F < 1.0 - 1e-6 && emp.x_sorted[i] > 0.0) {
            X.push_back(std::log(emp.x_sorted[i]));
            Y.push_back(std::log(std::log(1.0 / (1.0 - F))));
        }
    }
    if (X.size() < 2) throw std::runtime_error("Слишком мало точек для регрессии Вейбулла");

    // обычная МНК-прямая: Y = a + b*X → оценка b — наклон, a — свободный член
    double Sx = 0, Sy = 0, Sxx = 0, Sxy = 0; const double n = double(X.size());
    for (size_t i = 0; i < X.size(); ++i) { Sx += X[i]; Sy += Y[i]; Sxx += X[i] * X[i]; Sxy += X[i] * Y[i]; }
    double den = n * Sxx - Sx * Sx;
    if (std::fabs(den) < 1e-14) throw std::runtime_error("Вырождение в регрессии Вейбулла");
    double b = (n * Sxy - Sx * Sy) / den;
    double a = (Sy - b * Sx) / n;

    // для Weibull: Y = b*ln x - b*ln c → a = -b ln c → c = exp(-a/b)
    double c = std::exp(-a / b);

    if (!std::isfinite(b) || !std::isfinite(c) || b <= 0.0 || c <= 0.0)
        throw std::runtime_error("Регрессия Вейбулла дала некорректные параметры");

    return { c, b };
}


int main() {
    try {
        const string TAG = "MLE_Weibull";
        auto S = read_input_weibull(TAG);

        // Оценки ММП
        auto est = weibull_mle_2par(S.x, S.r);
        double c_hat = est.first;
        double b_hat = est.second;

        // Защита от NaN/Inf и плохих значений
        if (!(std::isfinite(c_hat) && std::isfinite(b_hat)) || c_hat <= 0.0 || b_hat <= 0.0) {
            // фолбэк на линейную регрессию на "бумаге Вейбулла"
            auto est_fb = weibull_regression_fallback(S.x, S.r);
            c_hat = est_fb.first;
            b_hat = est_fb.second;
        }
        // дополнительная "страховка" от диких чисел
        if (!(std::isfinite(c_hat) && std::isfinite(b_hat)) || c_hat <= 0.0 || b_hat <= 0.0) {
            throw std::runtime_error("Не удалось получить корректные оценки Вейбулла (b,c)");
        }


        // Эмпирика (К–М)
        auto emp = kaplan_meier_Itype(S.x, S.r);
        vector<double> X_emp = emp.x_sorted, Y_emp; Y_emp.reserve(emp.F_emp.size());
        for (double F : emp.F_emp) {
            double Fclip = std::min(std::max(F, 1e-12), 1.0 - 1e-12);
            double y = 5.0 + log(log(1.0 / (1.0 - Fclip)));
            Y_emp.push_back(y);
        }

        // Уровни p
        vector<double> P;
        {
            ifstream pin("Out/" + TAG + ".probs");
            for (string line; getline(pin, line);) if (!line.empty()) {
                for (char& c : line) if (c == ',') c = '.';
                try { P.push_back(stod(line)); }
                catch (...) {}
            }
        }

        const double beta = 0.95;
        const double z_beta = norm_ppf((1.0 + beta) / 2.0);

        auto Vpq = cov_weibull_asymp(S.x, S.r, c_hat, b_hat);

        vector<double> Xlow, Xest, Xupp, Yline;
        Xlow.reserve(P.size()); Xest.reserve(P.size()); Xupp.reserve(P.size()); Yline.reserve(P.size());

        for (double p : P) {
            double pclip = std::min(std::max(p, 1e-12), 1.0 - 1e-12);
            double t = -log(1.0 - pclip);
            double xp = c_hat * pow(t, 1.0 / b_hat);

            // x_p = exp(p) * t^{q} ; grad = [x_p, x_p*ln t] в координатах (p=ln c, q=1/b)
            double grad0 = xp;
            double grad1 = xp * log(t);
            double var_xp = (Vpq[0][0] * grad0 * grad0 + 2.0 * Vpq[0][1] * grad0 * grad1 + Vpq[1][1] * grad1 * grad1) / std::max(1, S.n);
            if (!std::isfinite(var_xp) || var_xp < 0.0) var_xp = 0.0;
            double sd_xp = sqrt(std::max(0.0, var_xp));

            // 🔸 Ограничим ошибку до 20% от значения квантиля (визуальное сглаживание)
            if (!std::isfinite(sd_xp) || sd_xp > 0.2 * xp)
                sd_xp = 0.2 * xp;



            Xest.push_back(xp);
            Xlow.push_back(xp - z_beta * sd_xp);
            Xupp.push_back(xp + z_beta * sd_xp);

            double y = 5.0 + log(log(1.0 / (1.0 - pclip)));
            Yline.push_back(y);
        }

        const int nc = 4;
        vector<int> m = { (int)X_emp.size(), (int)P.size(), (int)P.size(), (int)P.size() };

        ofstream xo("Out/" + TAG + ".xout");
        xo << nc << "\n";
        for (int i = 0; i < nc; i++) xo << m[i] << (i + 1 < nc ? " " : "");
        xo << "\n";
        xo << setprecision(17);
        for (double v : X_emp) xo << v << " "; xo << "\n";
        for (double v : Xlow) xo << v << " "; xo << "\n";
        for (double v : Xest) xo << v << " "; xo << "\n";
        for (double v : Xupp) xo << v << " "; xo << "\n";
        for (double v : Y_emp) xo << v << " "; xo << "\n";
        for (double v : Yline) xo << v << " "; xo << "\n";
        for (double v : Yline) xo << v << " "; xo << "\n";
        for (double v : Yline) xo << v << " "; xo << "\n";
        xo.close();

        ofstream out("Out/" + TAG + ".out");
        out << "Method:" << TAG << "\n";
        out << "n=" << S.n << "\n";
        out << "X\n"; for (double v : S.x) out << v << " , "; out << "\n";
        out << "R\n"; for (int v : S.r) out << v << " , "; out << "\n";
        out << fixed << setprecision(12);
        out << "c_hat=" << c_hat << "\n";
        out << "b_hat=" << b_hat << "\n";
        out << "P\n"; for (double p : P) out << p << " ; "; out << "\n";
        out << "Xp_low\n"; for (double v : Xlow) out << v << " ; "; out << "\n";
        out << "Xp\n"; for (double v : Xest) out << v << " ; "; out << "\n";
        out << "Xp_up\n"; for (double v : Xupp) out << v << " ; "; out << "\n";
        out.close();
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    }
}
