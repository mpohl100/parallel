#pragma once

#include "par/Work.h"
#include "par/Task.h"

#include <type_traits>

namespace par {

template<typename> class Calc2Impl;

template <typename TResult, typename TArg>
struct Calc2Impl<TResult(TArg)> : public Work {
  Calc2Impl() = default;
  Calc2Impl(std::function<TResult(TArg)> func) : _func{func} {}
  Calc2Impl(const Calc2Impl &) = default;
  Calc2Impl(Calc2Impl &&) = default;
  Calc2Impl &operator=(const Calc2Impl &) = default;
  Calc2Impl &operator=(Calc2Impl &&) = default;
  virtual ~Calc2Impl() = default;

  void call() override { _result = _func(_arg); }

  TResult result() const { return _result; }
  TArg arg() const { return _arg; }
  void set_arg(TArg arg) { _arg = arg; }
private:
  std::function<TResult(TArg)> _func;
  TResult _result;
  TArg _arg;
};

template<typename>
class Calc1Impl;

template <typename TResult>
struct Calc1Impl<TResult()> : public Work {
  Calc1Impl() = default;
  Calc1Impl(std::function<TResult()> func) : _func{func} {}
  Calc1Impl(const Calc1Impl &) = default;
  Calc1Impl(Calc1Impl &&) = default;
  Calc1Impl &operator=(const Calc1Impl &) = default;
  Calc1Impl &operator=(Calc1Impl &&) = default;
  virtual ~Calc1Impl() = default;

  void call() override { _result = _func(); }

  TResult result() const { return _result; }
private:
  std::function<TResult()> _func;
  TResult _result;
};

template <typename TArg>
struct Calc1Impl<void(TArg)> : public Work {
  Calc1Impl() = default;
  Calc1Impl(std::function<void(TArg)> func) : _func{func} {}
  Calc1Impl(const Calc1Impl &) = default;
  Calc1Impl(Calc1Impl &&) = default;
  Calc1Impl &operator=(const Calc1Impl &) = default;
  Calc1Impl &operator=(Calc1Impl &&) = default;
  virtual ~Calc1Impl() = default;

  void call() override { _func(_arg); }

  TArg arg() const { return _arg; }
  void set_arg(TArg arg) { _arg = arg; }
private:
  std::function<void(TArg)> _func;
  TArg _arg;
};

template<typename>
class CalcImpl;

template<>
struct CalcImpl<void()> : public Work {
  CalcImpl() = default;
  CalcImpl(std::function<void()> func) : _func{func} {}
  CalcImpl(const CalcImpl &) = default;
  CalcImpl(CalcImpl &&) = default;
  CalcImpl &operator=(const CalcImpl &) = default;
  CalcImpl &operator=(CalcImpl &&) = default;
  virtual ~CalcImpl() = default;

  void call() override { _func(); }

private:
  std::function<void()> _func;
};

template <typename> struct Calc2;


template <typename TResult, typename TArg> 
struct Calc2<TResult(TArg)> {
  Calc2() : _impl{std::make_shared<Calc2Impl<TResult(TArg)>>()} {}
  Calc2(std::function<TResult(TArg)> func)
      : _impl{std::make_shared<Calc2Impl<TResult(TArg)>>(func)} {}
  Calc2(const Calc2 &) = default;
  Calc2(Calc2 &&) = default;
  Calc2 &operator=(const Calc2 &) = default;
  Calc2 &operator=(Calc2 &&) = default;
  virtual ~Calc2() = default;
  Task make_task() const {
    return Task{_impl};
  }

private:
  std::shared_ptr<Calc2Impl<TResult(TArg)>> _impl;
};

template <typename> struct Calc1;

template <typename TResult> 
struct Calc1<TResult()> {
  Calc1() : _impl{std::make_shared<Calc1Impl<TResult()>>()} {}
  Calc1(std::function<TResult()> func)
      : _impl{std::make_shared<Calc1Impl<TResult()>>(func)} {}
  Calc1(const Calc1 &) = default;
  Calc1(Calc1 &&) = default;
  Calc1 &operator=(const Calc1 &) = default;
  Calc1 &operator=(Calc1 &&) = default;
  virtual ~Calc1() = default;
  Task make_task() const {
    return Task{_impl};
  }

private:
  std::shared_ptr<Calc1Impl<TResult()>> _impl;
};


template <typename TArg> 
struct Calc1<void(TArg)> {
  Calc1() : _impl{std::make_shared<Calc1Impl<void(TArg)>>()} {}
  Calc1(std::function<void(TArg)> func)
      : _impl{std::make_shared<Calc1Impl<void(TArg)>>(func)} {}
  Calc1(const Calc1 &) = default;
  Calc1(Calc1 &&) = default;
  Calc1 &operator=(const Calc1 &) = default;
  Calc1 &operator=(Calc1 &&) = default;
  virtual ~Calc1() = default;
  Task make_task() const {
    return Task{_impl};
  }

private:
  std::shared_ptr<Calc1Impl<void(TArg)>> _impl;
};

template<typename>
class Calc;

template<>
struct Calc<void()> {
  Calc() : _impl{std::make_shared<CalcImpl<void()>>()} {}
  Calc(std::function<void()> func)
      : _impl{std::make_shared<CalcImpl<void()>>(func)} {}
  Calc(const Calc &) = default;
  Calc(Calc &&) = default;
  Calc &operator=(const Calc &) = default;
  Calc &operator=(Calc &&) = default;
  virtual ~Calc() = default;
  Task make_task() const {
    return Task{_impl};
  }
private:
  std::shared_ptr<CalcImpl<void()>> _impl;
};

} // namespace par