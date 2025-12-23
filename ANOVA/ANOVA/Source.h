#pragma once

#include <vector>
#include <utility>
#include <random>
#include <string>

// Класс Sample: хранит выборку, её среднее и СКО
class Sample {
private:
    std::vector<double> sample;
    double mean = 0.0;
    double stdDev = 0.0;

public:
    // Конструктор от константной ссылки на вектор
    Sample(const std::vector<double>& sample);

    // Расчет среднего значения выборки
    double calculateMean() const;

    // Расчет выборочного стандартного отклонения (деление на n - 1)
    double calculateStandardDeviation() const;

    double getMean() const;
    double getStdDev() const;
    int getSampleSize() const;
    const std::vector<double>& getSample() const;
};

// Генератор mt19937 (глобальный)
std::mt19937& globalGenerator();

// Целое число в диапазоне [a, b]
int generateIntNumberFromAToB(std::mt19937& generator, int a = 20, int b = 45);

// Генерация одной нормальной выборки N(mean, stdDev)
std::vector<double> generateSample(double mean, double stdDev);

// Генерация списка выборок (групп)
std::vector<Sample> generateListOfSamples(int numberOfSamples);

// Общая средняя по всем группам
double calculateGeneralMean(const std::vector<Sample>& samples);

// Межгрупповая дисперсия
double calculateOuterDispersion(const std::vector<Sample>& samples,
    double generalMean);

// Внутригрупповая дисперсия
double calculateInnerDispersion(const std::vector<Sample>& samples);

// F-статистика
double calculateFisherStat(double sOut, double sIn);

// Степени свободы (f1, f2)
std::pair<int, int> calculateFreedomDegreees(const std::vector<Sample>& samples);

// Критическое значение F_alpha
double calculateFAlpha(const std::pair<int, int>& freedomDegrees, double alpha);

// Генерация входного файла и одновременное заполнение samples.
// Формат файла:
// первая строка: m – количество выборок;
// далее m строк: n_i x_{i1} x_{i2} ... x_{i n_i}
void generateInputFileIfEmpty(const std::string& fileName, int numberOfSamples, std::vector<Sample>& samples);

// Вывод результатов ANOVA в выходной файл.
// Файл каждый раз перезаписывается.
void writeAnovaToFile(const std::string& fileName, double alpha, const std::vector<Sample>& samples);



