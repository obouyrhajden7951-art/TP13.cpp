#include <iostream>
#include <memory>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include <cstddef>

// ---------- Exceptions specifiques ----------
class StackOverflow : public std::runtime_error {
public:
    StackOverflow() : std::runtime_error("StackOverflow : la pile est pleine") {}
};

class StackUnderflow : public std::runtime_error {
public:
    StackUnderflow() : std::runtime_error("StackUnderflow : la pile est vide") {}
};

// ---------- Pile generique ----------
template <typename T>
class Stack {
private:
    std::size_t capacity;
    std::size_t top;
    std::unique_ptr<T[]> data;

public:
    // Constructeur avec capacite par defaut
    explicit Stack(std::size_t cap = 16)
        : capacity(cap), top(0), data(new T[cap]) {}

    // Constructeur a partir d'une collection existante (begin, end)
    template <typename Iter>
    Stack(Iter first, Iter last)
        : capacity(std::max<std::size_t>(
              static_cast<std::size_t>(std::distance(first, last)), 1)),
          top(0), data(new T[capacity]) {
        for (; first != last; ++first)
            push(*first);
    }

    void push(const T& valeur) {
        if (top == capacity)
            throw StackOverflow();
        data[top++] = valeur;
    }

    void pop() {
        if (top == 0)
            throw StackUnderflow();
        --top;
    }

    const T& peek() const {
        if (top == 0)
            throw StackUnderflow();
        return data[top - 1];
    }

    bool empty() const { return top == 0; }
    std::size_t size() const { return top; }
};

int main() {
    // ---------- Utilisation simple ----------
    Stack<int> pile(3);
    pile.push(10);
    pile.push(20);
    pile.push(30);
    std::cout << "Sommet : " << pile.peek() << "\n";

    try {
        pile.push(40);
    } catch (const StackOverflow& e) {
        std::cout << "Exception : " << e.what() << "\n";
    }

    while (!pile.empty()) {
        std::cout << "pop " << pile.peek() << "\n";
        pile.pop();
    }

    try {
        pile.pop();
    } catch (const StackUnderflow& e) {
        std::cout << "Exception : " << e.what() << "\n";
    }

    // ---------- Construction a partir d'une collection ----------
    std::vector<std::string> mots = {"un", "deux", "trois", "quatre"};
    Stack<std::string> pileMots(mots.begin(), mots.end());
    std::cout << "Pile de mots, sommet : " << pileMots.peek()
              << " (taille " << pileMots.size() << ")\n";

    // ---------- Test avec un grand nombre d'elements ----------
    const std::size_t N = 1000000;
    Stack<int> grosse(N);
    for (std::size_t i = 0; i < N; i++)
        grosse.push(static_cast<int>(i));
    std::cout << "Grosse pile : " << grosse.size() << " elements, sommet = "
              << grosse.peek() << "\n";

    Stack<std::string> grosseStr(200000);
    for (std::size_t i = 0; i < 200000; i++)
        grosseStr.push("element numero " + std::to_string(i));
    std::cout << "Pile de strings : " << grosseStr.size()
              << " elements, sommet = " << grosseStr.peek() << "\n";

    // La memoire est liberee automatiquement par unique_ptr<T[]>
    return 0;
}
