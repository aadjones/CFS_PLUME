// Minimal test harness for CFS_PLUME. No external dependencies.
// Usage:
//   TEST(name) { ASSERT(cond); ASSERT_MSG(cond, "msg"); ASSERT_NEAR(a, b, tol); }
//   RUN_ALL_TESTS();  // in main()

#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <functional>

struct TestCase {
  std::string name;
  std::function<bool()> fn;
};

inline std::vector<TestCase>& testRegistry() {
  static std::vector<TestCase> tests;
  return tests;
}

struct TestRegistrar {
  TestRegistrar(const char* name, std::function<bool()> fn) {
    testRegistry().push_back({name, fn});
  }
};

#define TEST(name) \
  static bool test_##name(); \
  static TestRegistrar reg_##name(#name, test_##name); \
  static bool test_##name()

#define ASSERT(cond) do { \
  if (!(cond)) { \
    std::cerr << "  FAIL: " << #cond << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    return false; \
  } \
} while(0)

#define ASSERT_MSG(cond, msg) do { \
  if (!(cond)) { \
    std::cerr << "  FAIL: " << msg << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    return false; \
  } \
} while(0)

#define ASSERT_NEAR(a, b, tol) do { \
  double _a = (a), _b = (b), _t = (tol); \
  if (std::abs(_a - _b) > _t) { \
    std::cerr << "  FAIL: |" << _a << " - " << _b << "| = " << std::abs(_a - _b) \
              << " > " << _t << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    return false; \
  } \
} while(0)

#define INFO(msg) std::cout << "  [info] " << msg << std::endl

inline int runAllTests() {
  int passed = 0, failed = 0;
  for (auto& tc : testRegistry()) {
    std::cout << "[ RUN  ] " << tc.name << std::endl;
    bool ok = false;
    try {
      ok = tc.fn();
    } catch (const std::exception& e) {
      std::cerr << "  EXCEPTION: " << e.what() << std::endl;
    } catch (...) {
      std::cerr << "  UNKNOWN EXCEPTION" << std::endl;
    }
    if (ok) {
      std::cout << "[ PASS ] " << tc.name << std::endl;
      passed++;
    } else {
      std::cout << "[ FAIL ] " << tc.name << std::endl;
      failed++;
    }
  }
  std::cout << "\n========================================" << std::endl;
  std::cout << " " << passed << " passed, " << failed << " failed, "
            << (passed + failed) << " total" << std::endl;
  std::cout << "========================================" << std::endl;
  return failed > 0 ? 1 : 0;
}

#define RUN_ALL_TESTS() return runAllTests()

#endif
