#ifndef SINGLETON_H_
#define SINGLETON_H_

template<typename T>
class Singleton {
public:
    static T & Instance() {
        static T instance{token{}};
        return instance;
    }

    Singleton(const Singleton &) = delete;
    Singleton & operator= (const Singleton) = delete;

protected:
    struct token {};
    Singleton() {}
};

#endif  // SINGLETON_H_
