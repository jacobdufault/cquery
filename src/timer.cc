#include "timer.h"

#include <loguru.hpp>

using namespace std::chrono;

// static
long long Timer::GetCurrentTimeInMilliseconds() {
  auto time_since_epoch = Clock::now().time_since_epoch();
  return duration_cast<milliseconds>(time_since_epoch).count();
}

Timer::Timer() {
  Reset();
}

long long Timer::ElapsedMicroseconds() const {
  time_point<Clock> end = Clock::now();
  long long elapsed = elapsed_;
  if (start_.has_value()) {
    elapsed += duration_cast<microseconds>(end - *start_).count();
  }
  return elapsed;
}

long long Timer::ElapsedMicrosecondsAndReset() {
  long long elapsed = ElapsedMicroseconds();
  Reset();
  return elapsed;
}

void Timer::Reset() {
  start_ = Clock::now();
  elapsed_ = 0;
}

void Timer::ResetAndPrint(const std::string& message) {
  long long elapsed = ElapsedMicroseconds();
  long long milliseconds = elapsed / 1000;
  long long remaining = elapsed - milliseconds;
  LOG_S(INFO) << message << " took " << milliseconds << "." << remaining
              << "ms";
  Reset();
}

void Timer::Pause() {
  assert(start_.has_value());

  time_point<Clock> end = Clock::now();
  long long elapsed = duration_cast<microseconds>(end - *start_).count();

  elapsed_ += elapsed;
  start_ = nullopt;
}

void Timer::Resume() {
  assert(!start_.has_value());
  start_ = Clock::now();
}

ScopedPerfTimer::ScopedPerfTimer(const std::string& message)
    : message_(message) {}

ScopedPerfTimer::~ScopedPerfTimer() {
  timer_.ResetAndPrint(message_);
}
