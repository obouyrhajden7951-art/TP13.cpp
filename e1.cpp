#include <iostream>
#include <string>
#include <stdexcept>
#include <cstddef>

template <typename T>
class ArrayDyn {
private:
    T* data_;
    std::size_t capacity_;
    std::size_t size_;

    void reallouer(std::size_t nouvelleCapacite) {
        T* nouveau = new T[nouvelleCapacite];
        for (std::size_t i = 0; i < size_; i++)
            nouveau[i] = data_[i];
        delete[] data_;
        data_ = nouveau;
        capacity_ = nouvelleCapacite;
    }

public:
    // ---------- Iterateur bidirectionnel ----------
    template <typename U>
    class IterT {
    private:
        U* ptr_;
    public:
        explicit IterT(U* p = nullptr) : ptr_(p) {}

        U& operator*() const { return *ptr_; }
        U* operator->() const { return ptr_; }

        IterT& operator++() { ++ptr_; return *this; }
        IterT operator++(int) { IterT tmp = *this; ++ptr_; return tmp; }
        IterT& operator--() { --ptr_; return *this; }
        IterT operator--(int) { IterT tmp = *this; --ptr_; return tmp; }

        bool operator==(const IterT& autre) const { return ptr_ == autre.ptr_; }
        bool operator!=(const IterT& autre) const { return ptr_ != autre.ptr_; }
    };

    using Iterator = IterT<T>;
    using ConstIterator = IterT<const T>;

    // ---------- Constructeurs / destructeur (rule of three) ----------
    ArrayDyn() : data_(nullptr), capacity_(0), size_(0) {}

    explicit ArrayDyn(std::size_t capaciteInitiale)
        : data_(capaciteInitiale ? new T[capaciteInitiale] : nullptr),
          capacity_(capaciteInitiale), size_(0) {}

    ArrayDyn(const ArrayDyn& autre)
        : data_(autre.capacity_ ? new T[autre.capacity_] : nullptr),
          capacity_(autre.capacity_), size_(autre.size_) {
        for (std::size_t i = 0; i < size_; i++)
            data_[i] = autre.data_[i];
    }

    ArrayDyn& operator=(const ArrayDyn& autre) {
        if (this != &autre) {
            T* nouveau = autre.capacity_ ? new T[autre.capacity_] : nullptr;
            for (std::size_t i = 0; i < autre.size_; i++)
                nouveau[i] = autre.data_[i];
            delete[] data_;
            data_ = nouveau;
            capacity_ = autre.capacity_;
            size_ = autre.size_;
        }
        return *this;
    }

    ~ArrayDyn() { delete[] data_; }

    // ---------- Methodes ----------
    void push_back(const T& valeur) {
        if (size_ == capacity_)
            reallouer(capacity_ == 0 ? 4 : capacity_ * 2);
        data_[size_++] = valeur;
    }

    void pop_back() {
        if (size_ == 0)
            throw std::out_of_range("pop_back sur un tableau vide");
        --size_;
    }

    T& operator[](std::size_t index) {
        if (index >= size_)
            throw std::out_of_range("Index hors limites : " + std::to_string(index));
        return data_[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= size_)
            throw std::out_of_range("Index hors limites : " + std::to_string(index));
        return data_[index];
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }

    Iterator begin() { return Iterator(data_); }
    Iterator end() { return Iterator(data_ + size_); }
    ConstIterator begin() const { return ConstIterator(data_); }
    ConstIterator end() const { return ConstIterator(data_ + size_); }
};

// ---------- Objet maison pour les tests ----------
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << ", " << p.y << ")";
}

int main() {
    // Test avec int
    ArrayDyn<int> entiers;
    for (int i = 1; i <= 6; i++)
        entiers.push_back(i);

    std::cout << "Entiers : ";
    for (auto& e : entiers) {
        e *= 2;                      // modification via reference
        std::cout << e << " ";
    }
    std::cout << "\n";

    // Parcours inverse avec l'operateur --
    std::cout << "A l'envers : ";
    ArrayDyn<int>::Iterator it = entiers.end();
    while (it != entiers.begin()) {
        --it;
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Test avec std::string
    ArrayDyn<std::string> mots;
    mots.push_back("bonjour");
    mots.push_back("le");
    mots.push_back("monde");
    std::cout << "Mots : ";
    for (const auto& m : mots)
        std::cout << m << " ";
    std::cout << "\n";

    // Test avec un objet maison
    ArrayDyn<Point> points;
    points.push_back(Point(1, 2));
    points.push_back(Point(3, 4));
    std::cout << "Points : ";
    for (const auto& p : points)
        std::cout << p << " ";
    std::cout << "\n";

    // Test copie / affectation
    ArrayDyn<int> copie = entiers;
    ArrayDyn<int> autre;
    autre = entiers;
    copie[0] = 999;
    std::cout << "Original[0] = " << entiers[0] << ", copie[0] = " << copie[0]
              << ", autre[0] = " << autre[0] << "\n";

    // Test pop_back
    entiers.pop_back();
    std::cout << "Taille apres pop_back : " << entiers.size() << "\n";

    // Test des bornes
    try {
        std::cout << entiers[100] << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Exception : " << e.what() << "\n";
    }

    return 0;
}