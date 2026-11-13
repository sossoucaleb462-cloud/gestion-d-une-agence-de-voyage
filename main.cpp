#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

// ========================
// 1. CLASSE PASSAGER
// ========================
class Passager {
private:
    int id;
    string nom;
    string prenom;
    string telephone;
    string email;
    string passeport;

public:
    Passager(int id, string nom, string prenom, string telephone,
             string email, string passeport)
        : id(id), nom(nom), prenom(prenom), telephone(telephone),
          email(email), passeport(passeport) {}

    int getId() const { return id; }
    string getNomComplet() const { return prenom + " " + nom; }

    void afficher() const {
        cout << "ID : " << id
             << " | Nom : " << nom
             << " | Prenom : " << prenom
             << " | Telephone : " << telephone
             << " | Email : " << email
             << " | Passeport : " << passeport << '\n';
    }
};

// ========================
// 2. CLASSE VOL
// ========================
class Vol {
private:
    int id;
    string numero;
    string origine;
    string destination;
    string date;
    string heure;
    double prix;
    int capacite;
    int placesDisponibles;

public:
    Vol(int id, string numero, string origine, string destination,
        string date, string heure, double prix, int capacite)
        : id(id), numero(numero), origine(origine),
          destination(destination), date(date), heure(heure),
          prix(prix), capacite(capacite), placesDisponibles(capacite) {}

    int getId() const { return id; }
    string getNumero() const { return numero; }
    double getPrix() const { return prix; }
    int getPlacesDisponibles() const { return placesDisponibles; }

    bool retirerUnePlace() {
        if (placesDisponibles <= 0) return false;
        --placesDisponibles;
        return true;
    }

    void libererUnePlace() {
        if (placesDisponibles < capacite) ++placesDisponibles;
    }

    void afficher() const {
        cout << fixed << setprecision(2);
        cout << "ID : " << id
             << " | Vol : " << numero
             << " | " << origine << " -> " << destination
             << " | Date : " << date
             << " | Heure : " << heure
             << " | Prix : " << prix << " FCFA"
             << " | Places disponibles : " << placesDisponibles
             << "/" << capacite << '\n';
    }
};

// ========================
// 3. CLASSE PAIEMENT
// ========================
class Paiement {
private:
    int id;
    int reservationId;
    double montant;
    string date;
    string mode;

public:
    Paiement(int id, int reservationId, double montant,
             string date, string mode)
        : id(id), reservationId(reservationId), montant(montant),
          date(date), mode(mode) {}

    int getReservationId() const { return reservationId; }
    double getMontant() const { return montant; }

    void afficher() const {
        cout << "Paiement #" << id
             << " | Reservation #" << reservationId
             << " | Montant : " << montant << " FCFA"
             << " | Date : " << date
             << " | Mode : " << mode << '\n';
    }
};

// ========================
// 4. CLASSE RESERVATION
// ========================
class Reservation {
private:
    int id;
    int passagerId;
    int volId;
    double montantTotal;
    double montantPaye;
    string statut;

public:
    Reservation(int id, int passagerId, int volId, double montant)
        : id(id), passagerId(passagerId), volId(volId),
          montantTotal(montant), montantPaye(0.0),
          statut("EN_ATTENTE") {}

    int getId() const { return id; }
    int getPassagerId() const { return passagerId; }
    int getVolId() const { return volId; }
    double getMontantTotal() const { return montantTotal; }
    double getMontantPaye() const { return montantPaye; }
    string getStatut() const { return statut; }

    bool estAnnulee() const { return statut == "ANNULEE"; }

    bool ajouterPaiement(double montant) {
        if (estAnnulee() || montant <= 0 ||
            montantPaye + montant > montantTotal) {
            return false;
        }

        montantPaye += montant;

        if (montantPaye >= montantTotal) {
            statut = "CONFIRMEE";
        }

        return true;
    }

    void annuler() {
        statut = "ANNULEE";
    }

    void afficher() const {
        cout << fixed << setprecision(2);
        cout << "Reservation #" << id
             << " | Passager #" << passagerId
             << " | Vol #" << volId
             << " | Total : " << montantTotal
             << " FCFA | Paye : " << montantPaye
             << " FCFA | Statut : " << statut << '\n';
    }
};

// ========================
// 5. CLASSE AGENCE VOYAGE
// ========================
class AgenceVoyage {
private:
    vector<Passager> passagers;
    vector<Vol> vols;
    vector<Reservation> reservations;
    vector<Paiement> paiements;

    int prochainPassagerId = 1;
    int prochainVolId = 1;
    int prochaineReservationId = 1;
    int prochainPaiementId = 1;

    Passager* trouverPassager(int id) {
        for (auto& p : passagers)
            if (p.getId() == id) return &p;
        return nullptr;
    }

    Vol* trouverVol(int id) {
        for (auto& v : vols)
            if (v.getId() == id) return &v;
        return nullptr;
    }

    Reservation* trouverReservation(int id) {
        for (auto& r : reservations)
            if (r.getId() == id) return &r;
        return nullptr;
    }

public:
    // ----- Gestion des passagers -----
    void ajouterPassager() {
        string nom, prenom, telephone, email, passeport;

        cout << "\nNom : ";
        getline(cin >> ws, nom);
        cout << "Prenom : ";
        getline(cin, prenom);
        cout << "Telephone : ";
        getline(cin, telephone);
        cout << "Email : ";
        getline(cin, email);
        cout << "Numero de passeport : ";
        getline(cin, passeport);

        passagers.emplace_back(prochainPassagerId++, nom, prenom,
                               telephone, email, passeport);

        cout << "Passager ajoute avec succes.\n";
    }

    void afficherPassagers() const {
        cout << "\n--- LISTE DES PASSAGERS ---\n";
        if (passagers.empty()) {
            cout << "Aucun passager enregistre.\n";
            return;
        }
        for (const auto& p : passagers) p.afficher();
    }

    // ----- Gestion des vols -----
    void ajouterVol() {
        string numero, origine, destination, date, heure;
        double prix;
        int capacite;

        cout << "\nNumero du vol : ";
        getline(cin >> ws, numero);

        // Le numero d'un vol doit etre unique.
        for (const auto& v : vols) {
            if (v.getNumero() == numero) {
                cout << "Ce numero de vol existe deja.\n";
                return;
            }
        }

        cout << "Ville de depart : ";
        getline(cin, origine);
        cout << "Ville de destination : ";
        getline(cin, destination);
        cout << "Date (JJ/MM/AAAA) : ";
        getline(cin, date);
        cout << "Heure (HH:MM) : ";
        getline(cin, heure);
        cout << "Prix du billet (FCFA) : ";
        if (!(cin >> prix) || prix <= 0) {
            cout << "Prix invalide.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cout << "Capacite du vol : ";
        if (!(cin >> capacite) || capacite <= 0) {
            cout << "Capacite invalide.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        vols.emplace_back(prochainVolId++, numero, origine, destination,
                          date, heure, prix, capacite);
        cout << "Vol ajoute avec succes.\n";
    }

    void afficherVols() const {
        cout << "\n--- LISTE DES VOLS ---\n";
        if (vols.empty()) {
            cout << "Aucun vol enregistre.\n";
            return;
        }
        for (const auto& v : vols) v.afficher();
    }

    // ----- Gestion des reservations -----
    void reserverVol() {
        int passagerId, volId;

        afficherPassagers();
        if (passagers.empty()) return;

        cout << "\nID du passager : ";
        if (!(cin >> passagerId)) {
            cout << "Identifiant invalide.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        if (!trouverPassager(passagerId)) {
            cout << "Passager introuvable.\n";
            return;
        }

        afficherVols();
        if (vols.empty()) return;

        cout << "\nID du vol : ";
        if (!(cin >> volId)) {
            cout << "Identifiant invalide.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        Vol* vol = trouverVol(volId);
        if (!vol) {
            cout << "Vol introuvable.\n";
            return;
        }

        // Eviter deux reservations actives du meme passager sur le meme vol.
        for (const auto& r : reservations) {
            if (r.getPassagerId() == passagerId &&
                r.getVolId() == volId && !r.estAnnulee()) {
                cout << "Ce passager possede deja une reservation active pour ce vol.\n";
                return;
            }
        }

        if (!vol->retirerUnePlace()) {
            cout << "Aucune place disponible sur ce vol.\n";
            return;
        }

        reservations.emplace_back(prochaineReservationId++,
                                  passagerId, volId, vol->getPrix());

        cout << "Reservation creee. Elle est en attente de paiement.\n";
        cout << "Numero de reservation : "
             << reservations.back().getId() << '\n';
    }

    void afficherReservations() const {
        cout << "\n--- LISTE DES RESERVATIONS ---\n";
        if (reservations.empty()) {
            cout << "Aucune reservation enregistree.\n";
            return;
        }
        for (const auto& r : reservations) r.afficher();
    }

    // ----- Gestion des paiements -----
    void effectuerPaiement() {
        int reservationId;
        double montant;
        string date, mode;

        afficherReservations();
        if (reservations.empty()) return;

        cout << "\nNumero de reservation : ";
        if (!(cin >> reservationId)) {
            cout << "Identifiant invalide.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        Reservation* reservation = trouverReservation(reservationId);
        if (!reservation) {
            cout << "Reservation introuvable.\n";
            return;
        }

        if (reservation->estAnnulee()) {
            cout << "Impossible de payer une reservation annulee.\n";
            return;
        }

        double reste = reservation->getMontantTotal()
                     - reservation->getMontantPaye();

        cout << fixed << setprecision(2);
        cout << "Reste a payer : " << reste << " FCFA\n";
        cout << "Montant a payer : ";

        if (!(cin >> montant) || montant <= 0 || montant > reste) {
            cout << "Montant invalide. Le paiement doit etre positif et "
                 << "ne pas depasser le reste a payer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cout << "Date du paiement (JJ/MM/AAAA) : ";
        getline(cin >> ws, date);
        cout << "Mode de paiement (Especes/Mobile Money/Carte) : ";
        getline(cin, mode);

        if (!reservation->ajouterPaiement(montant)) {
            cout << "Paiement refuse.\n";
            return;
        }

        paiements.emplace_back(prochainPaiementId++, reservationId,
                               montant, date, mode);

        cout << "Paiement enregistre avec succes.\n";
        cout << "Nouveau statut : " << reservation->getStatut() << '\n';
    }

    void afficherPaiements() const {
        cout << "\n--- HISTORIQUE DES PAIEMENTS ---\n";
        if (paiements.empty()) {
            cout << "Aucun paiement enregistre.\n";
            return;
        }
        for (const auto& p : paiements) p.afficher();
    }

    // ----- Annulation -----
    void annulerReservation() {
        int id;
        afficherReservations();
        if (reservations.empty()) return;

        cout << "\nNumero de reservation a annuler : ";
        if (!(cin >> id)) {
            cout << "Identifiant invalide.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        Reservation* reservation = trouverReservation(id);
        if (!reservation) {
            cout << "Reservation introuvable.\n";
            return;
        }

        if (reservation->estAnnulee()) {
            cout << "Cette reservation est deja annulee.\n";
            return;
        }

        Vol* vol = trouverVol(reservation->getVolId());
        if (vol) vol->libererUnePlace();

        reservation->annuler();
        cout << "Reservation annulee. La place a ete liberee.\n";
        cout << "Attention : cette version ne gere pas automatiquement "
             << "le remboursement.\n";
    }

    // ----- Menu principal -----
    void menu() {
        int choix;

        do {
            cout << "\n====================================\n";
            cout << "     AGENCE DE VOYAGE - MENU\n";
            cout << "====================================\n";
            cout << "1. Ajouter un passager\n";
            cout << "2. Afficher les passagers\n";
            cout << "3. Ajouter un vol\n";
            cout << "4. Afficher les vols\n";
            cout << "5. Effectuer une reservation\n";
            cout << "6. Afficher les reservations\n";
            cout << "7. Effectuer un paiement\n";
            cout << "8. Afficher les paiements\n";
            cout << "9. Annuler une reservation\n";
            cout << "0. Quitter\n";
            cout << "Votre choix : ";

            if (!(cin >> choix)) {
                cout << "Veuillez saisir un nombre.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (choix) {
                case 1: ajouterPassager(); break;
                case 2: afficherPassagers(); break;
                case 3: ajouterVol(); break;
                case 4: afficherVols(); break;
                case 5: reserverVol(); break;
                case 6: afficherReservations(); break;
                case 7: effectuerPaiement(); break;
                case 8: afficherPaiements(); break;
                case 9: annulerReservation(); break;
                case 0: cout << "Fermeture de l'application.\n"; break;
                default: cout << "Choix invalide.\n";
            }
        } while (choix != 0);
    }
};

// ========================
// 6. FONCTION PRINCIPALE
// ========================
int main() {
    AgenceVoyage agence;
    agence.menu();
    return 0;
}