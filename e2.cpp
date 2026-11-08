#include <iostream>
#include <initializer_list>
#include <type_traits>
#include <cstddef>

template <typename T, std::size_t N, std::size_t M>
class Matrice {
private:
    T data[N][M];

public:
    // Constructeur par defaut : tout a 0 (false pour bool)
    Matrice() {
        for (std::size_t i = 0; i < N; i++)
            for (std::size_t j = 0; j < M; j++)
                data[i][j] = T();
    }

    // Constructeur par liste d'initialisation (remplissage ligne par ligne)
    Matrice(std::initializer_list<T> valeurs) : Matrice() {
        std::size_t k = 0;
        for (const T& v : valeurs) {
            if (k >= N * M) break;
            data[k / M][k % M] = v;
            k++;
        }
    }

    T& operator()(std::size_t i, std::size_t j) { return data[i][j]; }
    const T& operator()(std::size_t i, std::size_t j) const { return data[i][j]; }

    Matrice operator+(const Matrice& autre) const {
        Matrice res;
        for (std::size_t i = 0; i < N; i++)
            for (std::size_t j = 0; j < M; j++)
                res.data[i][j] = data[i][j] + autre.data[i][j];
        return res;
    }

    Matrice operator-(const Matrice& autre) const {
        Matrice res;
        for (std::size_t i = 0; i < N; i++)
            for (std::size_t j = 0; j < M; j++)
                res.data[i][j] = data[i][j] - autre.data[i][j];
        return res;
    }

    // Produit (N x M) * (M x P) = (N x P)
    template <std::size_t P>
    Matrice<T, N, P> operator*(const Matrice<T, M, P>& autre) const {
        Matrice<T, N, P> res;
        for (std::size_t i = 0; i < N; i++) {
            for (std::size_t j = 0; j < P; j++) {
                for (std::size_t k = 0; k < M; k++) {
                    if constexpr (std::is_same<T, bool>::value) {
                        // Produit logique : OU des ET
                        res(i, j) = res(i, j) || (data[i][k] && autre(k, j));
                    } else {
                        res(i, j) += data[i][k] * autre(k, j);
                    }
                }
            }
        }
        return res;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrice& m) {
        for (std::size_t i = 0; i < N; i++) {
            os << "[ ";
            for (std::size_t j = 0; j < M; j++)
                os << m.data[i][j] << " ";
            os << "]\n";
        }
        return os;
    }
};

int main() {
    // ---------- Entiers ----------
    Matrice<int, 2, 3> A = {1, 2, 3,
                            4, 5, 6};
    Matrice<int, 2, 3> B = {6, 5, 4,
                            3, 2, 1};
    Matrice<int, 3, 2> C = {1, 2,
                            3, 4,
                            5, 6};

    std::cout << "A + B =\n" << (A + B);
    std::cout << "A - B =\n" << (A - B);
    std::cout << "A * C (2x3 * 3x2 = 2x2) =\n" << (A * C);

    // ---------- Double ----------
    Matrice<double, 2, 2> D = {1.5, 2.0,
                               0.5, 1.0};
    Matrice<double, 2, 2> E = {2.0, 0.0,
                               1.0, 3.0};
    std::cout << "D + E =\n" << (D + E);
    std::cout << "D * E =\n" << (D * E);

    // ---------- Bool : produit logique ----------
    Matrice<bool, 2, 2> P = {true, false,
                             true, true};
    Matrice<bool, 2, 2> Q = {false, true,
                             true, false};
    std::cout << "P * Q (logique) =\n" << (P * Q);

    return 0;
}
