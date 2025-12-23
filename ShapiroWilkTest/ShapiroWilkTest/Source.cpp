#include "Source.h"

#include <numeric>
#include <cmath>
#include <fstream>


#include <boost/math/distributions/fisher_f.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/math/special_functions/beta.hpp>

// Реализация методов класса Sample

Sample::Sample(const std::vector<double>& sample)
    : sample(sample) {
    this->mean = calculateMean();
    this->stdDev = calculateStandardDeviation();
}

double Sample::calculateMean() const {
    double sum = std::accumulate(this->sample.begin(),
        this->sample.end(),
        0.0);
    const int n = static_cast<int>(this->sample.size());
    if (n == 0) {
        return 0.0;
    }
    return sum / static_cast<double>(n);
}

double Sample::calculateStandardDeviation() const {
    const int n = static_cast<int>(this->sample.size());
    if (n <= 1) {
        return 0.0;
    }

    double standartDeviation = 0.0;
    for (int i = 0; i < n; ++i) {
        double diff = this->sample[i] - this->mean;
        standartDeviation += diff * diff;
    }
    standartDeviation /= static_cast<double>(n - 1);
    standartDeviation = std::sqrt(standartDeviation);
    return standartDeviation;
}

double Sample::getMean() const {
    return this->mean;
}

double Sample::getStdDev() const {
    return this->stdDev;
}

int Sample::getSampleSize() const {
    return static_cast<int>(this->sample.size());
}

const std::vector<double>& Sample::getSample() const {
    return this->sample;
}


std::mt19937& globalGenerator() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

int generateIntNumberFromAToB(std::mt19937& generator, int a, int b) {
    std::uniform_int_distribution<int> uni(a, b);
    return uni(generator);
}

Sample generateSample(double mean, double stdDev) {
    // проверка на корректность СКО
    if (stdDev <= 0.0) {
        stdDev = 0.1;
    }

    std::mt19937& gen = globalGenerator();

    // генерация размера выборки
    const int n = generateIntNumberFromAToB(gen);

    std::vector<double> sample(n);

    // генерация выборки
    std::normal_distribution<double> dist(mean, stdDev);
    for (double& el : sample) {
        el = dist(gen);
    }
    std::sort(sample.begin(), sample.end()); // приведение к вариационному ряду
    return Sample(sample);
}

double calculateSquaredDeviations(const Sample& sample) {
    double res = 0;
    for (double el : sample.getSample()) {
        double diff = el - sample.getMean();
        res += diff*diff;
    }
    return res;
}


// ============================= ии слоп 💣💣💣💣💣💣==================================================

double integrateSimpson(const std::function<double(double)>& f,
    double a,
    double b,
    int steps) {

    if (steps % 2 != 0) {
        ++steps; // Симпсон требует чётное число шагов
    }

    const double h = (b - a) / steps;
    double sum_odd = 0.0;
    double sum_even = 0.0;

    for (int k = 1; k < steps; k += 2) {
        double x = a + k * h;
        sum_odd += f(x);
    }
    for (int k = 2; k < steps; k += 2) {
        double x = a + k * h;
        sum_even += f(x);
    }

    const double fa = f(a);
    const double fb = f(b);

    return (h / 3.0) * (fa + fb + 4.0 * sum_odd + 2.0 * sum_even);
}

// Функция F^{-1}(u,0,1) через boost
double normalQuantile01(double u) {
    static const boost::math::normal_distribution<double> nd(0.0, 1.0);
    return boost::math::quantile(nd, u);
}

// α_{n,r} по формуле (2.72)
double alpha_nr(int n, int r) {
    const double eps = 1e-8; // чтобы не лезть в quantile(0) и quantile(1)

    auto integrand = [=](double x) {
        double z = normalQuantile01(x); // F^{-1}(x,0,1)
        double w = std::pow(1.0 - x, n - r) * std::pow(x, r - 1);
        return z * w;
        };

    double numerator = integrateSimpson(integrand, eps, 1.0 - eps);
    double denom = boost::math::beta(static_cast<double>(r),
        static_cast<double>(n - r + 1));
    return numerator / denom;
}

// V_{r,r} (дисперсия) по формуле (2.73)
double V_rr(int n, int r, double alpha_nr_value) {
    const double eps = 1e-8;

    auto integrand = [=](double x) {
        double z = normalQuantile01(x);
        double w = std::pow(1.0 - x, n - r) * std::pow(x, r - 1);
        return z * z * w;
        };

    double numerator = integrateSimpson(integrand, eps, 1.0 - eps);
    double denom = boost::math::beta(static_cast<double>(r),
        static_cast<double>(n - r + 1));
    double EX2 = numerator / denom;

    return EX2 - alpha_nr_value * alpha_nr_value;
}

// Внутренний интеграл по u в формуле (2.74)
double innerIntegral_for_Vrs(double v, int r, int s) {
    const double eps = 1e-8;
    if (v <= 2.0 * eps) {
        return 0.0;
    }

    auto inner = [=](double u) {
        double zu = normalQuantile01(u);
        double t1 = std::pow(u, r - 1);
        double t2 = std::pow(v - u, s - r - 1);
        return t1 * zu * t2;
        };

    return integrateSimpson(inner, eps, v - eps);
}

// V_{r,s} (ковариация) по формуле (2.74), r < s
double V_rs(int n, int r, int s,
    double alpha_r, double alpha_s) {
    const double eps = 1e-8;

    auto outer = [=](double v) {
        double zv = normalQuantile01(v);
        double first = zv * std::pow(1.0 - v, n - s);
        double inner = innerIntegral_for_Vrs(v, r, s);
        return first * inner;
        };

    double numerator = integrateSimpson(outer, 2.0 * eps, 1.0 - eps);

    double denom = boost::math::beta(static_cast<double>(r),
        static_cast<double>(s - r))
        * boost::math::beta(static_cast<double>(s),
            static_cast<double>(n - s + 1));

    double EXrXs = numerator / denom;
    return EXrXs - alpha_r * alpha_s;
}


// Решение V y = b методом Гаусса (без частичного выбора опорного)
std::vector<double> solveLinearSystem(std::vector<std::vector<double>> V,
    std::vector<double> b) {
    int n = static_cast<int>(V.size());

    for (int i = 0; i < n; ++i) {
        // Пивот по диагонали (очень упрощённо – можно улучшать)
        double pivot = V[i][i];
        if (std::fabs(pivot) < 1e-14) {
            throw std::runtime_error("Pivot is too small in Gaussian elimination");
        }

        // Нормализуем строку
        for (int j = i; j < n; ++j) {
            V[i][j] /= pivot;
        }
        b[i] /= pivot;

        // Обнуляем в остальных строках столбец i
        for (int k = 0; k < n; ++k) {
            if (k == i) continue;
            double factor = V[k][i];
            for (int j = i; j < n; ++j) {
                V[k][j] -= factor * V[i][j];
            }
            b[k] -= factor * b[i];
        }
    }

    return b; // теперь b — решение y
}

// Полный расчёт коэффициентов a_i через интегралы
std::vector<double> computeAi_viaIntegrals(int n) {
    // 1. α_r
    std::vector<double> alpha(n);
    for (int r = 1; r <= n; ++r) {
        alpha[r - 1] = alpha_nr(n, r);
    }

    // 2. Матрица V
    std::vector<std::vector<double>> V(n, std::vector<double>(n, 0.0));

    // диагональ
    for (int r = 1; r <= n; ++r) {
        V[r - 1][r - 1] = V_rr(n, r, alpha[r - 1]);
    }

    // внедиагональные элементы (симметрия)
    for (int r = 1; r <= n; ++r) {
        for (int s = r + 1; s <= n; ++s) {
            double cov = V_rs(n, r, s, alpha[r - 1], alpha[s - 1]);
            V[r - 1][s - 1] = cov;
            V[s - 1][r - 1] = cov;
        }
    }

    // 3. y = V^{-1} alpha
    std::vector<double> y = solveLinearSystem(V, alpha);

    // 4. Нормировка
    double norm2 = 0.0;
    for (double val : y) {
        norm2 += val * val;
    }
    double norm = std::sqrt(norm2);

    std::vector<double> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = y[i] / norm;
    }

    return a;
}

// ============================= конец ии слопа 💣💣💣💣💣💣==================================================


double calculateBCoef(const std::vector<double>& a_i, const Sample& sample) {
    double result = 0;
    for (std::size_t i = 0; i < sample.getSampleSize(); i++) {
        result += a_i[i] * sample.getSample()[i];
    }
    return result;
}

double calculateWilkStat(const double b, const double sSquared) {
    return (b * b) / sSquared;
}

double calculateWAlpha(int n) {
    // Табличные значения W_crit для уровня значимости alpha = 0.05
    // по Приложению П1 (n = 3..50).
    // Индекс массива = n, элементы 0,1,2 не используются.
    static const double Wcrit05[51] = {
        0.0,   // 0 - нет теста
        0.0,   // 1 - нет теста
        0.0,   // 2 - нет теста
        0.767, // 3
        0.748, // 4
        0.762, // 5
        0.788, // 6
        0.803, // 7
        0.818, // 8
        0.829, // 9
        0.842, // 10
        0.850, // 11
        0.859, // 12
        0.866, // 13
        0.874, // 14
        0.881, // 15
        0.887, // 16
        0.892, // 17
        0.897, // 18
        0.901, // 19
        0.905, // 20
        0.908, // 21
        0.911, // 22
        0.914, // 23
        0.916, // 24
        0.918, // 25
        0.920, // 26
        0.923, // 27
        0.924, // 28
        0.926, // 29
        0.927, // 30
        0.929, // 31
        0.930, // 32
        0.931, // 33
        0.933, // 34
        0.934, // 35
        0.935, // 36
        0.936, // 37
        0.938, // 38
        0.939, // 39
        0.940, // 40
        0.941, // 41
        0.942, // 42
        0.943, // 43
        0.944, // 44
        0.945, // 45
        0.945, // 46
        0.946, // 47
        0.947, // 48
        0.947, // 49
        0.947  // 50
    };

    if (n < 3 || n > 50) {
        throw std::invalid_argument(
            "Критические значения W по Агамирову заданы только для 3 <= n <= 50");
    }

    return Wcrit05[n];
}

Sample generateInputFileWithNewSample(const std::string& filePath,
    double mean,
    double stdDev) {

    Sample sample = generateSample(mean, stdDev);

    std::ofstream fout(filePath, std::ios::trunc);
    if (!fout.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filePath);
    }

    const int n = sample.getSampleSize();
    const std::vector<double>& data = sample.getSample();

    fout << n << '\n';
    for (int i = 0; i < n; ++i) {
        fout << data[i];
        if (i + 1 < n) {
            fout << ' ';
        }
    }
    fout << '\n';

    return sample;
}

void writeShapiroWilkResultToFile(const std::string& filePath,
    const Sample& sample,
    const std::vector<double>& a_i,
    double alpha) {

    std::ofstream fout(filePath, std::ios::trunc);
    if (!fout.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filePath);
    }

    const int n = sample.getSampleSize();
    const double mean = sample.getMean();
    const double stdDev = sample.getStdDev();

    double sSquared = calculateSquaredDeviations(sample);
    double b = calculateBCoef(a_i, sample);
    double W_observed = calculateWilkStat(b, sSquared);
    double W_crit = calculateWAlpha(n);

    fout << "Критерий Шапиро-Уилка\n";
    fout << "Уровень значимости alpha = " << alpha << "\n\n";

    fout << "Размер выборки n = " << n << "\n";
    fout << "Выборочное среднее x̄ = " << mean << "\n";
    fout << "Выборочное СКО s = " << stdDev << "\n\n";

    fout << "Сумма квадратов отклонений s^2 = " << sSquared << "\n";
    fout << "Коэффициент b = " << b << "\n";
    fout << "Наблюдаемое значение статистики Wнабл = " << W_observed << "\n";
    fout << "Критическое значение Wкр = " << W_crit << "\n\n";

    if (W_observed >= W_crit) {
        fout << "Вывод: Wнабл > Wкр, нет оснований отвергать нулевую гипотезу.\n";
        fout << "Распределение можно считать нормальным.\n";
    }
    else {
        fout << "Вывод: Wнабл < Wкр, нулевая гипотеза отвергается.\n";
        fout << "Распределение нельзя считать нормальным.\n";
    }
}
