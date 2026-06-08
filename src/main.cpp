// Copyright 2022 NNTU-CS
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "tree.h"

std::vector<char> generateChars(int cnt) {
  std::vector<char> chars;
  for (int idx = 0; idx < cnt; ++idx) {
    chars.push_back(static_cast<char>('1' + idx));
  }
  return chars;
}

uint64_t factorialNum(int n) {
  uint64_t f = 1;
  for (int k = 2; k <= n; ++k) {
    f *= k;
  }
  return f;
}

void displayPerm(const std::vector<char>& perm) {
  for (char ch : perm) {
    std::cout << ch;
  }
  std::cout << std::endl;
}

void demoPerms() {
  std::cout << "\n========== ДЕМОНСТРАЦИЯ РАБОТЫ ФУНКЦИЙ ==========\n"
            << std::endl;

  std::vector<char> input1 = {'1', '2', '3'};
  std::cout << "Создаем дерево для символов: ";
  for (char c : input1) {
    std::cout << c << " ";
  }
  std::cout << std::endl;

  PermutationTree ptree(input1);

  std::cout << "\n--- allPermutations() ---" << std::endl;
  auto allPerms = ptree.allPermutations();
  std::cout << "Всего перестановок: " << allPerms.size() << std::endl;
  std::cout << "Перестановки:" << std::endl;
  for (size_t i = 0; i < allPerms.size(); ++i) {
    std::cout << i + 1 << ": ";
    displayPerm(allPerms[i]);
  }

  std::cout << "\n--- getPermSlow() и getPermFast() ---" << std::endl;

  std::cout << "Перестановка №1 (getPermSlow): ";
  auto p1 = getPermSlow(ptree, 1);
  displayPerm(p1);

  std::cout << "Перестановка №2 (getPermFast): ";
  auto p2 = getPermFast(ptree, 2);
  displayPerm(p2);

  std::cout << "Перестановка №3 (getPermSlow): ";
  auto p3 = getPermSlow(ptree, 3);
  displayPerm(p3);

  std::cout << "Перестановка №6 (getPermFast): ";
  auto p6 = getPermFast(ptree, 6);
  displayPerm(p6);

  std::cout << "\nПроверка обработки ошибок:" << std::endl;
  std::cout << "Перестановка №10 (не существует): ";
  auto badPerm = getPermSlow(ptree, 10);
  if (badPerm.empty()) {
    std::cout << "вернут пустой вектор (корректно)" << std::endl;
  }

  std::cout << "\n--- Пример с 4 символами (1,3,5,7) ---" << std::endl;
  std::vector<char> input2 = {'1', '3', '5', '7'};
  PermutationTree ptree2(input2);

  std::cout << "Первые 5 перестановок:" << std::endl;
  for (int idx = 1; idx <= 5; ++idx) {
    auto perm = getPermFast(ptree2, idx);
    std::cout << idx << ": ";
    displayPerm(perm);
  }
}

template<typename Func>
double measureTimeMs(Func fn) {
  auto tStart = std::chrono::high_resolution_clock::now();
  fn();
  auto tEnd = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> diff = tEnd - tStart;
  return diff.count();
}

void runExp() {
  std::cout << "\n\n========== ВЫЧИСЛИТЕЛЬНЫЙ ЭКСПЕРИМЕНТ ==========\n"
            << std::endl;

  const int MIN_N = 1;
  const int MAX_N = 10;

  std::random_device rd;
  std::mt19937 rng(rd());

  std::cout << std::setw(5) << "n"
            << std::setw(15) << "n!"
            << std::setw(20) << "allPerms(ms)"
            << std::setw(20) << "getPermSlow(ms)"
            << std::setw(20) << "getPermFast(ms)"
            << std::endl;
  std::cout << std::string(80, '-') << std::endl;

  for (int n = MIN_N; n <= MAX_N; ++n) {
    std::vector<char> syms = generateChars(n);

    std::cout << "n=" << n << " (построение дерева...) " << std::flush;
    PermutationTree ptree(syms);
    std::cout << "готово" << std::endl;

    uint64_t totalPerms = factorialNum(n);

    std::uniform_int_distribution<uint64_t> dist(1, totalPerms);
    int chosenNum = static_cast<int>(dist(rng));

    double tAll = measureTimeMs([&]() {
      ptree.allPermutations();
    });

    double tSlow = measureTimeMs([&]() {
      getPermSlow(ptree, chosenNum);
    });

    double tFast = measureTimeMs([&]() {
      getPermFast(ptree, chosenNum);
    });

    std::cout << std::setw(5) << n
              << std::setw(15) << totalPerms
              << std::setw(20) << std::fixed << std::setprecision(3) << tAll
              << std::setw(20) << tSlow
              << std::setw(20) << tFast
              << std::endl;
  }

  std::cout << "\n========== АНАЛИЗ РЕЗУЛЬТАТОВ ==========\n" << std::endl;
  std::cout << "Наблюдения:\n";
  std::cout << "1. allPermutations() генерирует все n! перестановок, "
            << "время растет факториально\n";
  std::cout << "2. getPermSlow() также выполняет полный перебор всех "
            << "перестановок, что крайне неэффективно\n";
  std::cout << "3. getPermFast() использует навигацию по дереву и работает "
            << "за O(n) независимо от n!\n";
  std::cout << "4. При больших n разница между getPermFast и другими методами "
            << "составляет несколько порядков\n";
}

int main() {
  demoPerms();
  runExp();
  std::cout << "\nПрограмма завершена." << std::endl;
  return 0;
}
