#include <assert.h>

#include <iostream>

#include "../headers/Engine.h"
#include "../headers/Wrapper.h"

class Talker {
   public:
    Talker(std::string const&& s = "No Name") : name(s) {}

    void sayMyName() { std::cout << name << std::endl; }

    void saySomeNumber(int n) {
        std::cout << "This is number: " << n << std::endl;
    }

    int returnDiff(int a, int b) { return a - b; }

    template <typename... Args>
    void sayALotWord(Args... args) {
        (std::cout << ... << args) << std::endl;
    }

    void numberAndString(int n, std::string s) {
        std::cout << s << n << std::endl;
    }

   private:
    std::string name;
};

void ExecuteWithoutParameters() {
    Engine eng;
    Talker tl("Hussenberg");

    Wrapper wp(&tl, &Talker::sayMyName);
    eng.register_command(&wp, "say name");
    eng.execute("say name");
}

void ExecuteWithOneParameterDefault() {
    Engine eng;
    Talker tl;

    Wrapper wp(&tl, &Talker::saySomeNumber, {{"n", 42}});
    eng.register_command(&wp, "say num");
    eng.execute("say num");
}

void ExecuteWithOneParameterNoDefault() {
    Engine eng;
    Talker tl;

    Wrapper wp(&tl, &Talker::saySomeNumber, {{"n", 45}});
    eng.register_command(&wp, "say num");
    eng.execute("say num");
}

void ExecuteWithParametersWithDifferentTypes() {
    Engine eng;
    Talker tl;

    Wrapper wp(&tl, &Talker::numberAndString,
               {{"n", 1}, {"s", std::string("R")}});
    eng.register_command(&wp, "sn");
    eng.execute("sn");
}

int negativeNumber(int a) { return -a; }

void ExecuteWithReturn() {
    Engine eng;
    Talker tl;

    Wrapper wp(&tl, &Talker::returnDiff, {{"b", 5}, {"a", 2}});

    eng.register_command(&wp, "diff");

    std::cout << eng.execute("diff", {{"a", -100}}) << std::endl;

    int res = eng.execute("diff", {{"a", -50}});
    assert(res == 55);

    std::cout << negativeNumber(eng.execute("diff", {{"a", -100}}))
              << std::endl;
}

void allIsDynamic() {
    Engine* eng = new Engine();
    Talker* tl = new Talker("Some Name");

    auto wp = new Wrapper(tl, &Talker::sayMyName);
    eng->register_command(wp, "say");
    eng->execute("say");

    delete eng, tl, wp;
}

int main() {
    ExecuteWithoutParameters();
    ExecuteWithOneParameterDefault();
    ExecuteWithOneParameterNoDefault();
    ExecuteWithParametersWithDifferentTypes();
    ExecuteWithReturn();
    allIsDynamic();

    return 0;
}