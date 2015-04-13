//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if defined(_MSC_VER)

#include <boost/di.hpp>
#include <cassert>

namespace di = boost::di;

auto blah = []{};
auto my_int = []{};

struct i {
    virtual ~i() noexcept = default;
    virtual void dummy() = 0;
};

struct i2 {
    virtual ~i2() noexcept = default;
    virtual void dummy2() = 0;
};

struct impl2 : i2 {
    void dummy2() override {}
};

struct i3 {
    virtual ~i3() noexcept = default;
    virtual void dummy3() = 0;
};

struct impl3 : i3 {
    BOOST_DI_INJECT(impl3, (named = my_int) const int& i, double) {
        assert(i == 21);
    }
    void dummy3() override {}
};

struct impl : i{
    impl(int i, std::shared_ptr<i2>){
        assert(i == 87);
    }
    void dummy() override {};
};

struct c {
    BOOST_DI_INJECT(c, int, int, int, double, double, std::shared_ptr<i>, (named = blah) std::shared_ptr<i3>){}
};

struct module {
    di::injector<i> configure() const {
        return di::make_injector(
                di::bind<i, impl>()
              , di::bind<i2, impl2>()
              , di::bind<int>().to(87)
        );
    }
};

test ft = []{
    auto injector = di::make_injector(
          di::bind<int>().to(42)
        , module{}
        , di::bind<i3, impl3>().named(blah)
        , di::bind<int>().named(my_int).to(21)
    );

    auto object = injector.create<i*>();
    assert(object != nullptr);
    assert(42 == injector.create<int>());

    injector.create<c>();
};

#endif

