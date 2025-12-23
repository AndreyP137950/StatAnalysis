#include <limits>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <direct.h>

#include "Source.hpp"

#undef max

using namespace std;


int run_normal();
int run_weibull();

// -------------------------------------------------
// main: выбор распределения по препроцессорному define
// -------------------------------------------------
int main(int, const char**) {
    setlocale(LC_ALL, "");
    setlocale(LC_NUMERIC, "C");

#ifdef NORMAL
    return run_normal();
#elif defined(WEIBULL)
    return run_weibull();
#else
#error "Не выбрано распределение: задайте NORMAL или WEIBULL в определениях препроцессора"
#endif
}

// НОРМАЛЬНОЕ РАСПРЕДЕЛЕНИЕ
int run_normal() {
    const string TAG = "mle_normal";
    auto S = read_input_normal(TAG);

    char cwd[1024];
    _getcwd(cwd, sizeof(cwd));
    cout << "Рабочая папка: " << cwd << endl;

    // наблюдения без цензуры
    vector<double> obs;
    obs.reserve(S.n);
    for (int i = 0; i < S.n; ++i) {
        if (S.r[i] == 0) {
            obs.push_back(S.x[i]);
        }
    }

    // начальные приближения
    double a0 = 0.0;
    double s0 = 1.0;

    if (!obs.empty()) {
        a0 = accumulate(obs.begin(), obs.end(), 0.0) / static_cast<double>(obs.size());
        double q = 0.0;
        for (double v : obs) {
            q += (v - a0) * (v - a0);
        }
        if (obs.size() > 1) {
            s0 = sqrt(q / static_cast<double>(obs.size() - 1));
        }
        else {
            s0 = 1.0;
        }
    }
    else {
        vector<double> xs = S.x;
        sort(xs.begin(), xs.end());
        a0 = xs[xs.size() / 2];
        s0 = std::max(1e-3, (xs.back() - xs.front()) / 6.0);
    }

    vector<double> theta;
    theta.push_back(a0);
    theta.push_back(std::max(1e-6, s0));

    const bool censored = (static_cast<int>(obs.size()) != S.n);

    if (censored) {
        G_X = &S.x;
        G_R = &S.r;
        neldermead(theta, 1e-10, &normal_score_fn);
    }

    double a_hat = 0.0;
    double s_hat = 1.0;

    if (!censored) {
        a_hat = accumulate(S.x.begin(), S.x.end(), 0.0) / static_cast<double>(S.n);
        double q = 0.0;
        for (double v : S.x) {
            q += (v - a_hat) * (v - a_hat);
        }
        if (S.n > 0) {
            s_hat = sqrt(q / static_cast<double>(S.n));
        }
        else {
            s_hat = theta[1];
        }
        if (!(s_hat > 0.0)) {
            s_hat = theta[1];
        }
    }
    else {
        a_hat = theta[0];
        s_hat = theta[1];
    }

    // эмпирика К–М
    EmpiricalKM emp = kaplan_meier_Itype(S.x, S.r);
    vector<double> X_emp = emp.x_sorted;
    vector<double> Y_emp;
    Y_emp.reserve(emp.F_emp.size());
    for (size_t i = 0; i < emp.F_emp.size(); ++i) {
        double F = emp.F_emp[i];
        double Fclip = std::min(std::max(F, 1.0e-12), 1.0 - 1.0e-12);
        double z = norm_ppf(Fclip);
        Y_emp.push_back(5.0 + z);
    }

    // уровни вероятностей
    vector<double> P;
    {
        ifstream pin("Out/" + TAG + ".probs");
        string line;
        while (std::getline(pin, line)) {
            if (line.empty()) {
                continue;
            }
            for (size_t i = 0; i < line.size(); ++i) {
                if (line[i] == ',') {
                    line[i] = '.';
                }
            }
            try {
                double p = std::stod(line);
                P.push_back(p);
            }
            catch (...) {
                // пропускаем строки, которые не парсятся
            }
        }
    }

    // асимптотическая ковариационная матрица
    vector<vector<double> > V = cov_normal_asymp(S.x, S.r, a_hat, s_hat);
    const double beta = 0.95;
    const double z_beta = norm_ppf((1.0 + beta) / 2.0);

    vector<double> Xlow;
    vector<double> Xest;
    vector<double> Xupp;
    vector<double> Yline;

    Xlow.reserve(P.size());
    Xest.reserve(P.size());
    Xupp.reserve(P.size());
    Yline.reserve(P.size());

    for (size_t i = 0; i < P.size(); ++i) {
        double p_raw = P[i];
        double p = std::min(std::max(p_raw, 1.0e-12), 1.0 - 1.0e-12);
        double z = norm_ppf(p);
        double xp = a_hat + z * s_hat;

        double var_xp = V[0][0] + 2.0 * V[0][1] * z + V[1][1] * z * z;
        var_xp = std::max(0.0, var_xp / static_cast<double>(S.n));
        double sd_xp = std::sqrt(var_xp);

        Xest.push_back(xp);
        Xlow.push_back(xp - z_beta * sd_xp);
        Xupp.push_back(xp + z_beta * sd_xp);
        Yline.push_back(5.0 + z);
    }

    // запись .xout: 7 рядов
    const int nc = 7;
    vector<int> m;
    m.push_back(static_cast<int>(X_emp.size()));
    m.push_back(static_cast<int>(P.size()));
    m.push_back(static_cast<int>(P.size()));
    m.push_back(static_cast<int>(P.size()));
    m.push_back(static_cast<int>(Y_emp.size()));
    m.push_back(static_cast<int>(Yline.size()));
    m.push_back(static_cast<int>(Yline.size()));

    ofstream xo("Out/" + TAG + ".xout");
    if (!xo) {
        cerr << "[ERROR] Не удалось открыть Out/" << TAG << ".xout\n";
        return 1;
    }

    xo << nc << "\n";
    for (int i = 0; i < nc; ++i) {
        xo << m[i];
        if (i + 1 < nc) {
            xo << " ";
        }
    }
    xo << "\n";
    xo << std::setprecision(17);

    // 1. эмпирические X
    for (size_t i = 0; i < X_emp.size(); ++i) {
        xo << X_emp[i] << " ";
    }
    xo << "\n";

    // 2. нижние границы
    for (size_t i = 0; i < Xlow.size(); ++i) {
        xo << Xlow[i] << " ";
    }
    xo << "\n";

    // 3. оценки
    for (size_t i = 0; i < Xest.size(); ++i) {
        xo << Xest[i] << " ";
    }
    xo << "\n";

    // 4. верхние границы
    for (size_t i = 0; i < Xupp.size(); ++i) {
        xo << Xupp[i] << " ";
    }
    xo << "\n";

    // 5. эмпирические Y
    for (size_t i = 0; i < Y_emp.size(); ++i) {
        xo << Y_emp[i] << " ";
    }
    xo << "\n";

    // 6. теоретическая линия
    for (size_t i = 0; i < Yline.size(); ++i) {
        xo << Yline[i] << " ";
    }
    xo << "\n";

    // 7. дубликат теоретической линии
    for (size_t i = 0; i < Yline.size(); ++i) {
        xo << Yline[i] << " ";
    }
    xo << "\n";

    xo.close();

    // .out
    ofstream out("Out/" + TAG + ".out");
    out << "Method:" << TAG << "\n";
    out << "n=" << S.n << "\n";

    out << "X\n";
    for (size_t i = 0; i < S.x.size(); ++i) {
        out << S.x[i] << " , ";
    }
    out << "\n";

    out << "R\n";
    for (size_t i = 0; i < S.r.size(); ++i) {
        out << S.r[i] << " , ";
    }
    out << "\n";

    out << std::fixed << std::setprecision(12);
    out << "a_hat=" << a_hat << "\n";
    out << "sigma_hat=" << s_hat << "\n";
    out << "Cov[a,s]:\n";
    out << V[0][0] << " " << V[0][1] << "\n";
    out << V[1][0] << " " << V[1][1] << "\n";

    out << "P\n";
    for (size_t i = 0; i < P.size(); ++i) {
        out << P[i] << " ; ";
    }
    out << "\n";

    out << "Xp_low\n";
    for (size_t i = 0; i < Xlow.size(); ++i) {
        out << Xlow[i] << " ; ";
    }
    out << "\n";

    out << "Xp\n";
    for (size_t i = 0; i < Xest.size(); ++i) {
        out << Xest[i] << " ; ";
    }
    out << "\n";

    out << "Xp_up\n";
    for (size_t i = 0; i < Xupp.size(); ++i) {
        out << Xupp[i] << " ; ";
    }
    out << "\n";

    out.close();

    cout << "Выполнено: " << TAG << endl;
    return 0;
}

// -------------------------------------------------
// ВЕИБУЛЛОВСКОЕ РАСПРЕДЕЛЕНИЕ
// -------------------------------------------------
int run_weibull() {
    try {
        const string TAG = "mle_weibull";
        auto S = read_input_weibull(TAG);

        char cwd[1024];
        _getcwd(cwd, sizeof(cwd));
        cout << "Рабочая папка: " << cwd << endl;

        // основные оценки ММП
        std::pair<double, double> est = weibull_mle_2par(S.x, S.r);
        double c_hat = est.first;
        double b_hat = est.second;

        if (!(std::isfinite(c_hat) && std::isfinite(b_hat)) ||
            c_hat <= 0.0 || b_hat <= 0.0) {

            std::pair<double, double> est_fb =
                weibull_regression_fallback(S.x, S.r);

            c_hat = est_fb.first;
            b_hat = est_fb.second;
        }

        if (!(std::isfinite(c_hat) && std::isfinite(b_hat)) ||
            c_hat <= 0.0 || b_hat <= 0.0) {

            throw std::runtime_error("Не удалось получить корректные оценки Вейбулла (b,c)");
        }

        // эмпирика К–М: X = ln(t), Y = ln(-ln(1-F))
        EmpiricalKM emp = kaplan_meier_Itype(S.x, S.r);

        vector<double> X_emp_log;
        vector<double> Y_emp;

        X_emp_log.reserve(emp.x_sorted.size());
        Y_emp.reserve(emp.F_emp.size());

        for (size_t i = 0; i < emp.x_sorted.size(); ++i) {
            double t = emp.x_sorted[i];
            double F = emp.F_emp[i];

            if (t <= 0.0) {
                continue;
            }

            double Fclip = std::min(std::max(F, 1.0e-12), 1.0 - 1.0e-12);
            double x = std::log(t);
            double y = std::log(-std::log(1.0 - Fclip));

            X_emp_log.push_back(x);
            Y_emp.push_back(y);
        }

        // уровни p для теоретики
        vector<double> P;
        {
            std::ifstream pin("Out/" + TAG + ".probs");
            std::string line;
            while (std::getline(pin, line)) {
                if (line.empty()) {
                    continue;
                }
                for (size_t i = 0; i < line.size(); ++i) {
                    if (line[i] == ',') {
                        line[i] = '.';
                    }
                }
                try {
                    double p = std::stod(line);
                    P.push_back(p);
                }
                catch (...) {
                    // пропускаем непарсящиеся строки
                }
            }
        }

        const double beta = 0.95;
        const double z_beta = norm_ppf((1.0 + beta) / 2.0);

        // асимптотическая ковариация в координатах (p = ln c, q = 1/b)
        std::pair< std::vector<std::vector<double> >, int >
            V_eff = cov_weibull_asymp_eff(S.x, S.r, c_hat, b_hat);

        std::vector<std::vector<double> >& Vpq = V_eff.first;
        const int n_eff = (V_eff.second > 0 ? V_eff.second : 1);

        // квантильные линии в лог-шкале ln t
        vector<double> Xlow_log;
        vector<double> Xest_log;
        vector<double> Xupp_log;
        vector<double> Yline;

        Xlow_log.reserve(P.size());
        Xest_log.reserve(P.size());
        Xupp_log.reserve(P.size());
        Yline.reserve(P.size());

        for (size_t i = 0; i < P.size(); ++i) {
            double p_raw = P[i];
            double pclip = std::min(std::max(p_raw, 1.0e-12), 1.0 - 1.0e-12);

            double t = -std::log(1.0 - pclip);
            double xp = c_hat * std::pow(t, 1.0 / b_hat); // квантиль по времени

            // градиент по (p = ln c, q = 1/b)
            double g0 = xp;
            double g1 = xp * std::log(t);

            double var_xp =
                Vpq[0][0] * g0 * g0 +
                2.0 * Vpq[0][1] * g0 * g1 +
                Vpq[1][1] * g1 * g1;

            var_xp = std::max(0.0, var_xp / static_cast<double>(n_eff));
            double sd_xp = std::sqrt(var_xp);

            double x_low = std::max(xp - z_beta * sd_xp, 1.0e-12);
            double x_upp = std::max(xp + z_beta * sd_xp, 1.0e-12);

            Xest_log.push_back(std::log(std::max(xp, 1.0e-12)));
            Xlow_log.push_back(std::log(x_low));
            Xupp_log.push_back(std::log(x_upp));

            double y = std::log(-std::log(1.0 - pclip));
            Yline.push_back(y);
        }

        // запись .xout (7 рядов, как у нормального случая)
        const int nc = 7;
        vector<int> m;
        m.push_back(static_cast<int>(X_emp_log.size())); // 1
        m.push_back(static_cast<int>(P.size()));         // 2
        m.push_back(static_cast<int>(P.size()));         // 3
        m.push_back(static_cast<int>(P.size()));         // 4
        m.push_back(static_cast<int>(Y_emp.size()));     // 5
        m.push_back(static_cast<int>(Yline.size()));     // 6
        m.push_back(static_cast<int>(Yline.size()));     // 7

        std::ofstream xo("Out/" + TAG + ".xout");
        if (!xo) {
            std::cerr << "[ERROR] Не удалось открыть Out/" << TAG << ".xout\n";
            return 1;
        }

        xo << nc << "\n";
        for (int i = 0; i < nc; ++i) {
            xo << m[i];
            if (i + 1 < nc) {
                xo << " ";
            }
        }
        xo << "\n";
        xo << std::setprecision(17);

        // 1. эмпирические X (ln t)
        for (size_t i = 0; i < X_emp_log.size(); ++i) {
            xo << X_emp_log[i] << " ";
        }
        xo << "\n";

        // 2. нижняя граница квантилей (ln t_low)
        for (size_t i = 0; i < Xlow_log.size(); ++i) {
            xo << Xlow_log[i] << " ";
        }
        xo << "\n";

        // 3. оценочные квантили (ln t_hat)
        for (size_t i = 0; i < Xest_log.size(); ++i) {
            xo << Xest_log[i] << " ";
        }
        xo << "\n";

        // 4. верхняя граница квантилей (ln t_upp)
        for (size_t i = 0; i < Xupp_log.size(); ++i) {
            xo << Xupp_log[i] << " ";
        }
        xo << "\n";

        // 5. эмпирические Y
        for (size_t i = 0; i < Y_emp.size(); ++i) {
            xo << Y_emp[i] << " ";
        }
        xo << "\n";

        // 6. теоретическая линия
        for (size_t i = 0; i < Yline.size(); ++i) {
            xo << Yline[i] << " ";
        }
        xo << "\n";

        // 7. дубликат теоретической линии
        for (size_t i = 0; i < Yline.size(); ++i) {
            xo << Yline[i] << " ";
        }
        xo << "\n";

        xo.close();

        // .out
        std::ofstream out("Out/" + TAG + ".out");
        out << "Method:" << TAG << "\n";
        out << "n=" << S.n << "\n";

        out << "X\n";
        for (size_t i = 0; i < S.x.size(); ++i) {
            out << S.x[i] << " , ";
        }
        out << "\n";

        out << "R\n";
        for (size_t i = 0; i < S.r.size(); ++i) {
            out << S.r[i] << " , ";
        }
        out << "\n";

        out << std::fixed << std::setprecision(12);
        out << "c_hat=" << c_hat << "\n";
        out << "b_hat=" << b_hat << "\n";

        out << "P\n";
        for (size_t i = 0; i < P.size(); ++i) {
            out << P[i] << " ; ";
        }
        out << "\n";

        out << "Xp_low_log\n";
        for (size_t i = 0; i < Xlow_log.size(); ++i) {
            out << Xlow_log[i] << " ; ";
        }
        out << "\n";

        out << "Xp_log\n";
        for (size_t i = 0; i < Xest_log.size(); ++i) {
            out << Xest_log[i] << " ; ";
        }
        out << "\n";

        out << "Xp_up_log\n";
        for (size_t i = 0; i < Xupp_log.size(); ++i) {
            out << Xupp_log[i] << " ; ";
        }
        out << "\n";

        out.close();

        std::cout << "Выполнено: " << TAG
            << " (c=" << c_hat << ", b=" << b_hat << ")" << std::endl;

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    }
}
