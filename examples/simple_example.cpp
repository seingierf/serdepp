#include <serdepp/serializer.hpp>
#include <serdepp/adaptor/nlohmann_json.hpp>
#include <serdepp/adaptor/toml11.hpp>
#include <serdepp/adaptor/yaml-cpp.hpp>
#include <serdepp/adaptor/fmt.hpp>
#include <serdepp/ostream.hpp>

#include <serdepp/attributes.hpp>

using namespace serde::ostream;

enum class tenum {
    INPUT ,
    OUTPUT,
};

class test {
public:
    template<class Context>
    constexpr static auto serde(Context& context, test& value) {
        using serde::attribute::set_default;
        serde::serde_struct(context, value)
            .field(&test::str, "str")
            .field(&test::i,   "i")
            .field(&test::vec, "vec")
            .field(&test::io,  "io")
            .field(&test::pri, "pri")
            .field(&test::m ,  "m")
            ;
    }
    std::optional<std::string> str;
    int i;
    std::optional<std::vector<std::string>> vec;
    tenum io;
    std::map<std::string, std::string> m;
private:
    std::string pri;
};

int main()
{
  nlohmann::json v = R"({
"i": 10,
"vec": [ "one", "two", "three" ],
"io": "INPUT",
"pri" : "pri",
"m" : { "a" : "1",
        "b" : "2",
        "c" : "3" }
})"_json;

  try {
    test t = serde::serialize<test>(v);

    auto v_to_json = serde::deserialize<nlohmann::json>(t);
    auto v_to_toml = serde::deserialize<serde::toml_v>(t);
    auto v_to_yaml = serde::deserialize<serde::yaml>(t);

    std::cout << "toml: " << v_to_toml << std::endl;
    fmt::print("json: {}\n", v_to_json.dump());
    std::cout << "yaml: " << v_to_yaml << std::endl;

    test t_from_toml = serde::serialize<test>(v_to_toml);
    test t_from_yaml = serde::serialize<test>(v_to_yaml);


    fmt::print("{}\n", t_from_toml);
    fmt::print("{}\n", t_from_yaml);
    std::cout << t << '\n';

  }catch(std::exception& e) {
      fmt::print(stderr,"{}\n",e.what());
  }

  return 0;
}