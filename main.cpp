#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

// ======================================
// 1. COULEURS
// ======================================

enum Couleur {
    ROUGE,
    NOIR
};

// ======================================
// 2. STRUCTURE D'UN NOEUD
// ======================================

struct Noeud {

    int valeur;
    Couleur couleur;

    Noeud* gauche;
    Noeud* droite;
    Noeud* parent;

    Noeud(int v) {
        valeur = v;
        couleur = ROUGE;
        gauche = nullptr;
        droite = nullptr;
        parent = nullptr;
    }
};

// ======================================
// 3. CLASSE ARBRE ROUGE-NOIR
// ======================================

class ArbreRougeNoir {

private:

    Noeud* racine;

    // ======================================
    // DESTRUCTEUR RECURSIF
    // ======================================

    void detruireRecursif(Noeud* courant) {

        if (courant == nullptr)
            return;

        detruireRecursif(courant->gauche);
        detruireRecursif(courant->droite);

        delete courant;
    }

    // ======================================
    // ROTATION GAUCHE
    // ======================================

    void rotationGauche(Noeud* x) {

        Noeud* y = x->droite;

        x->droite = y->gauche;

        if (y->gauche != nullptr)
            y->gauche->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            racine = y;

        else if (x == x->parent->gauche)
            x->parent->gauche = y;

        else
            x->parent->droite = y;

        y->gauche = x;
        x->parent = y;
    }

    // ======================================
    // ROTATION DROITE
    // ======================================

    void rotationDroite(Noeud* x) {

        Noeud* y = x->gauche;

        x->gauche = y->droite;

        if (y->droite != nullptr)
            y->droite->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            racine = y;

        else if (x == x->parent->gauche)
            x->parent->gauche = y;

        else
            x->parent->droite = y;

        y->droite = x;
        x->parent = y;
    }

    // ======================================
    // CORRECTION APRES INSERTION
    // ======================================

    void corrigerInsertion(Noeud* z) {

        while (z != racine && z->parent->couleur == ROUGE) {

            Noeud* p = z->parent;
            Noeud* g = p->parent;

            if (p == g->gauche) {

                Noeud* oncle = g->droite;

                if (oncle != nullptr && oncle->couleur == ROUGE) {

                    p->couleur = NOIR;
                    oncle->couleur = NOIR;
                    g->couleur = ROUGE;

                    z = g;
                }
                else {

                    if (z == p->droite) {
                        z = p;
                        rotationGauche(z);
                    }

                    z->parent->couleur = NOIR;
                    z->parent->parent->couleur = ROUGE;

                    rotationDroite(z->parent->parent);
                }
            }
            else {

                Noeud* oncle = g->gauche;

                if (oncle != nullptr && oncle->couleur == ROUGE) {

                    p->couleur = NOIR;
                    oncle->couleur = NOIR;
                    g->couleur = ROUGE;

                    z = g;
                }
                else {

                    if (z == p->gauche) {
                        z = p;
                        rotationDroite(z);
                    }

                    z->parent->couleur = NOIR;
                    z->parent->parent->couleur = ROUGE;

                    rotationGauche(z->parent->parent);
                }
            }
        }

        racine->couleur = NOIR;
    }

    // ======================================
    // RECHERCHE INTERNE
    // ======================================

    Noeud* rechercherNoeud(int valeur) {

        Noeud* courant = racine;

        while (courant != nullptr) {

            if (valeur == courant->valeur)
                return courant;

            if (valeur < courant->valeur)
                courant = courant->gauche;
            else
                courant = courant->droite;
        }

        return nullptr;
    }

    // ======================================
    // TROUVER LE MINIMUM
    // ======================================

    Noeud* minimum(Noeud* courant) {

        while (courant->gauche != nullptr)
            courant = courant->gauche;

        return courant;
    }

    // ======================================
    // REMPLACER UN NOEUD
    // ======================================

    void transplanter(Noeud* u, Noeud* v) {

        if (u->parent == nullptr)
            racine = v;

        else if (u == u->parent->gauche)
            u->parent->gauche = v;

        else
            u->parent->droite = v;

        if (v != nullptr)
            v->parent = u->parent;
    }

    // ======================================
    // VERIFIER SI UN NOEUD EST NOIR
    // ======================================

    bool estNoir(Noeud* n) {

        return n == nullptr || n->couleur == NOIR;
    }

    // ======================================
    // CORRECTION APRES SUPPRESSION
    // ======================================

    void corrigerSuppression(Noeud* x, Noeud* parent, bool estFilsGauche) {

        while (x != racine && estNoir(x)) {

            if (parent == nullptr)
                break;

            bool gauche = (x != nullptr)
                          ? (x == parent->gauche)
                          : estFilsGauche;

            if (gauche) {

                Noeud* frere = parent->droite;

                // CAS 1 : FRERE ROUGE
                if (frere != nullptr && frere->couleur == ROUGE) {

                    frere->couleur = NOIR;
                    parent->couleur = ROUGE;

                    rotationGauche(parent);

                    frere = parent->droite;
                }

                // CAS 2 : LES DEUX FILS DU FRERE SONT NOIRS
                if (frere == nullptr ||
                    (estNoir(frere->gauche) && estNoir(frere->droite))) {

                    if (frere != nullptr)
                        frere->couleur = ROUGE;

                    x = parent;
                    parent = x->parent;

                    if (parent != nullptr)
                        estFilsGauche = (x == parent->gauche);
                }

                else {

                    // CAS 3
                    if (estNoir(frere->droite)) {

                        if (frere->gauche != nullptr)
                            frere->gauche->couleur = NOIR;

                        frere->couleur = ROUGE;

                        rotationDroite(frere);

                        frere = parent->droite;
                    }

                    // CAS 4
                    frere->couleur = parent->couleur;
                    parent->couleur = NOIR;

                    if (frere->droite != nullptr)
                        frere->droite->couleur = NOIR;

                    rotationGauche(parent);

                    x = racine;
                    parent = nullptr;
                }
            }

            else {

                Noeud* frere = parent->gauche;

                // CAS 1 : FRERE ROUGE
                if (frere != nullptr && frere->couleur == ROUGE) {

                    frere->couleur = NOIR;
                    parent->couleur = ROUGE;

                    rotationDroite(parent);

                    frere = parent->gauche;
                }

                // CAS 2
                if (frere == nullptr ||
                    (estNoir(frere->gauche) && estNoir(frere->droite))) {

                    if (frere != nullptr)
                        frere->couleur = ROUGE;

                    x = parent;
                    parent = x->parent;

                    if (parent != nullptr)
                        estFilsGauche = (x == parent->gauche);
                }

                else {

                    // CAS 3
                    if (estNoir(frere->gauche)) {

                        if (frere->droite != nullptr)
                            frere->droite->couleur = NOIR;

                        frere->couleur = ROUGE;

                        rotationGauche(frere);

                        frere = parent->gauche;
                    }

                    // CAS 4
                    frere->couleur = parent->couleur;
                    parent->couleur = NOIR;

                    if (frere->gauche != nullptr)
                        frere->gauche->couleur = NOIR;

                    rotationDroite(parent);

                    x = racine;
                    parent = nullptr;
                }
            }
        }

        if (x != nullptr)
            x->couleur = NOIR;
    }

    // ======================================
    // SUPPRESSION INTERNE
    // ======================================

    void supprimerNoeud(Noeud* z) {

        Noeud* y = z;
        Noeud* x;
        Noeud* parentX;
        bool estFilsGauche;

        Couleur ancienneCouleur = y->couleur;

        // CAS 1 : PAS DE FILS GAUCHE
        if (z->gauche == nullptr) {

            x = z->droite;
            parentX = z->parent;

            estFilsGauche = (parentX != nullptr &&
                             z == parentX->gauche);

            transplanter(z, z->droite);

            if (x != nullptr) {
                parentX = x->parent;
                estFilsGauche = (parentX != nullptr &&
                                 x == parentX->gauche);
            }
        }

        // CAS 2 : PAS DE FILS DROIT
        else if (z->droite == nullptr) {

            x = z->gauche;
            parentX = z->parent;

            estFilsGauche = (parentX != nullptr &&
                             z == parentX->gauche);

            transplanter(z, z->gauche);

            if (x != nullptr) {
                parentX = x->parent;
                estFilsGauche = (parentX != nullptr &&
                                 x == parentX->gauche);
            }
        }

        // CAS 3 : DEUX FILS
        else {

            y = minimum(z->droite);
            ancienneCouleur = y->couleur;

            x = y->droite;

            if (y->parent == z) {

                parentX = y;
                estFilsGauche = false;

                if (x != nullptr)
                    x->parent = y;
            }
            else {

                parentX = y->parent;
                estFilsGauche = (y == parentX->gauche);

                transplanter(y, y->droite);

                y->droite = z->droite;
                y->droite->parent = y;
            }

            transplanter(z, y);

            y->gauche = z->gauche;
            y->gauche->parent = y;

            y->couleur = z->couleur;
        }

        delete z;

        if (ancienneCouleur == NOIR)
            corrigerSuppression(x, parentX, estFilsGauche);
    }

public:

    // ======================================
    // CONSTRUCTEUR
    // ======================================

    ArbreRougeNoir() {
        racine = nullptr;
    }

    // ======================================
    // DESTRUCTEUR
    // ======================================

    ~ArbreRougeNoir() {
        detruireRecursif(racine);
    }

    // Empêcher la copie superficielle de l'arbre
    ArbreRougeNoir(const ArbreRougeNoir&) = delete;
    ArbreRougeNoir& operator=(const ArbreRougeNoir&) = delete;

    // ======================================
    // INSERTION
    // ======================================

    void inserer(int valeur) {

        Noeud* nouveau = new Noeud(valeur);

        Noeud* parent = nullptr;
        Noeud* courant = racine;

        while (courant != nullptr) {

            parent = courant;

            if (valeur < courant->valeur)
                courant = courant->gauche;

            else if (valeur > courant->valeur)
                courant = courant->droite;

            else {
                delete nouveau;
                return;
            }
        }

        nouveau->parent = parent;

        if (parent == nullptr)
            racine = nouveau;

        else if (valeur < parent->valeur)
            parent->gauche = nouveau;

        else
            parent->droite = nouveau;

        corrigerInsertion(nouveau);
    }

    // ======================================
    // RECHERCHE PUBLIQUE
    // ======================================

    bool rechercher(int valeur) {

        return rechercherNoeud(valeur) != nullptr;
    }

    // ======================================
    // SUPPRESSION PUBLIQUE
    // ======================================

    bool supprimer(int valeur) {

        Noeud* n = rechercherNoeud(valeur);

        if (n == nullptr)
            return false;

        supprimerNoeud(n);

        return true;
    }
};

// ======================================
// 4. PROGRAMME PRINCIPAL
// ======================================

int main() {

    int tailles[] = {
        100, 1000, 10000, 100000, 1000000,10000000,50000000,100000000
    };

    random_device rd;
    mt19937 generateur(rd());

    cout << fixed << setprecision(3);

    cout << "\n";
    cout << "============================================================\n";
    cout << "             ARBRE ROUGE-NOIR - COMPARAISON\n";
    cout << "============================================================\n";

    cout << left
         << setw(15) << "Nombre N"
         << setw(20) << "Insertion (ms)"
         << setw(20) << "Recherche (ms)"
         << setw(20) << "Suppression (ms)"
         << endl;

    cout << "------------------------------------------------------------\n";

    for (int n : tailles) {

        vector<int> nombres;

        // Generer les nombres de 1 a N
        for (int i = 1; i <= n; i++) {
            nombres.push_back(i);
        }

        // Melanger les nombres aleatoirement
        shuffle(nombres.begin(), nombres.end(), generateur);

        ArbreRougeNoir arbre;

        // ======================================
        // MESURE DE L'INSERTION
        // ======================================

        auto debutInsertion = high_resolution_clock::now();

        for (int nombre : nombres) {
            arbre.inserer(nombre);
        }

        auto finInsertion = high_resolution_clock::now();

        double tempsInsertion =
            duration<double, milli>(
                finInsertion - debutInsertion
            ).count();

        // ======================================
        // MESURE DE LA RECHERCHE
        // ======================================

        auto debutRecherche = high_resolution_clock::now();

        for (int nombre : nombres) {
            arbre.rechercher(nombre);
        }

        auto finRecherche = high_resolution_clock::now();

        double tempsRecherche =
            duration<double, milli>(
                finRecherche - debutRecherche
            ).count();

        // ======================================
        // MESURE DE LA SUPPRESSION
        // ======================================

        auto debutSuppression = high_resolution_clock::now();

        for (int nombre : nombres) {
            arbre.supprimer(nombre);
        }

        auto finSuppression = high_resolution_clock::now();

        double tempsSuppression =
            duration<double, milli>(
                finSuppression - debutSuppression
            ).count();

        // ======================================
        // AFFICHAGE DES RESULTATS
        // ======================================

        cout << left
             << setw(15) << n
             << setw(20) << tempsInsertion
             << setw(20) << tempsRecherche
             << setw(20) << tempsSuppression
             << endl;

        cout << "------------------------------------------------------------\n";
    }

    cout << "\nFin des tests." << endl;

    return 0;
}
