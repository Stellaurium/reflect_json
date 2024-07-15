这个项目来源于
https://www.bilibili.com/list/watchlater?bvid=BV1bf421Q7H4&oid=1205824279
https://github.com/archibate/reflect-hpp
但是是完全自己书写的. 在学习了代码之后 自己实践书写的

目的是可以实现c++的自动序列化和反序列化


v2.0 版本删掉了 教程一般的注释，如果未来可能看不懂代码了，看v1.0版本的注释进行回忆

Version 2.0 的核心逻辑，剩下的都只是将这个东西封装起来，方便使用而已
```cpp
class Person {
  public:
    std::string name;
    int age;

    template <typename Func>
    static void for_each_member_ptr(Func &&func) {
        func("name", &Person::name);
        func("age", &Person::age);
    }
};

using json = nlohmann::json;

template <typename T>
std::string serialize(const T &object) {
    json root;
    // 传递的都是指针 可以使用 只能使用auto 不需要引用
    T::for_each_member_ptr([&](const std::string &key, auto ptr) { root[key] = object.*ptr; });
    return root.dump(4);
}

TEST_CASE("v2.0 test serialization") {
    Person person{"Jay", 12};
    fmt::println("{}", serialize(person));
}
```