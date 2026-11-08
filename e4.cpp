#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <concepts>
#include <utility>

// ---------- Concept ----------
template <typename T>
concept Sortable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
};

// ---------- Foncteurs de comparaison ----------
template <typename T>
struct Asc {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <typename T>
struct Desc {
    bool operator()(const T& a, const T& b) const { return b < a; }
};

// ---------- Quick sort ----------
template <typename T, typename Comp>
void quickSortImpl(std::vector<T>& v, long bas, long haut, Comp& comp) {
    if (bas >= haut) return;

    const T pivot = v[haut];
    long i = bas - 1;
    for (long j = bas; j < haut; j++) {
        if (comp(v[j], pivot)) {
            ++i;
            std::swap(v[i], v[j]);
        }
    }
    std::swap(v[i + 1], v[haut]);
    long p = i + 1;

    quickSortImpl(v, bas, p - 1, comp);
    quickSortImpl(v, p + 1, haut, comp);
}

template <Sortable T, typename Comp = std::less<T>>
void quickSort(std::vector<T>& v, Comp comp = Comp{}) {
    if (v.size() < 2) return;
    quickSortImpl(v, 0, static_cast<long>(v.size()) - 1, comp);
}

// ---------- Structure utilisateur ----------
struct Personne {
    std::string nom;
    int age;

    bool operator<(const Personne& autre) const { return age < autre.age; }
};

std::ostream& operator<<(std::ostream& os, const Personne& p) {
    return os << p.nom << "(" << p.age << ")";
}

// ---------- Outils de test ----------
template <typename T, typename Comp>
bool estTrie(const std::vector<T>& v, Comp comp) {
    for (std::size_t i = 1; i < v.size(); i++)
        if (comp(v[i], v[i - 1]))
            return false;
    return true;
}

template <typename T>
void afficher(const std::string& titre, const std::vector<T>& v) {
    std::cout << titre << " : ";
    for (const auto& e : v)
        std::cout << e << " ";
    std::cout << "\n";
}

void verifier(bool condition, const std::string& nomTest) {
    std::cout << (condition ? "[OK]   " : "[ECHEC] ") << nomTest << "\n";
}

int main() {
    // ---------- int ----------
    std::vector<int> entiers = {5, 2, 9, 1, 5, 6, -3, 0};
    quickSort(entiers);                       // par defaut : std::less
    afficher("int croissant", entiers);
    verifier(estTrie(entiers, Asc<int>{}), "vector<int> trie (croissant)");

    quickSort(entiers, Desc<int>{});
    afficher("int decroissant", entiers);
    verifier(estTrie(entiers, Desc<int>{}), "vector<int> trie (decroissant)");

    // ---------- string ----------
    std::vector<std::string> mots = {"pomme", "banane", "cerise", "abricot", "kiwi"};
    quickSort(mots, Asc<std::string>{});
    afficher("string croissant", mots);
    verifier(estTrie(mots, Asc<std::string>{}), "vector<string> trie (croissant)");

    quickSort(mots, Desc<std::string>{});
    afficher("string decroissant", mots);
    verifier(estTrie(mots, Desc<std::string>{}), "vector<string> trie (decroissant)");

    // ---------- structure utilisateur ----------
    std::vector<Personne> gens = {
        {"Ali", 30}, {"Sara", 22}, {"Omar", 41}, {"Lina", 27}
    };
    quickSort(gens, Asc<Personne>{});
    afficher("Personne par age croissant", gens);
    verifier(estTrie(gens, Asc<Personne>{}), "vector<Personne> trie (croissant)");

    quickSort(gens, Desc<Personne>{});
    afficher("Personne par age decroissant", gens);
    verifier(estTrie(gens, Desc<Personne>{}), "vector<Personne> trie (decroissant)");

    // ---------- cas limites ----------
    std::vector<int> vide;
    quickSort(vide);
    verifier(vide.empty(), "vecteur vide");

    std::vector<int> un = {42};
    quickSort(un);
    verifier(un.size() == 1 && un[0] == 42, "vecteur a un element");

    return 0;
}
