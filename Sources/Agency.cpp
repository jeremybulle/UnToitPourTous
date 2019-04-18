#include <memory>

#include <memory>

#include <memory>

//
// Created by eliot on 25/02/19.
//

#include <iostream>
#include <memory>
#include <limits>
#include <fstream>
#include <algorithm>
#include "../Headers/Agency.h"
#include "../Headers/Goods/Good.h"
#include "../Headers/Goods/House.h"
#include "../Headers/Goods/Flat.h"
#include "../Headers/Goods/Ground.h"
#include "../Headers/Goods/Professional.h"
#include "../Headers/Clients/Client.h"
#include "../Headers/Clients/Buyer.h"
#include "../Headers/Clients/Seller.h"
#include "../Headers/Utils.h"

using namespace std;

shared_ptr<Good> Agency::addGood() {
    cout << "Vous allez rajouter un nouveau bien.\n";
    std::shared_ptr<Seller> sellerRef = getSellerRef();
    string sellerName = sellerRef->getName();

    shared_ptr<Good> toAdd;

    int goodKind = Utils::selectType();

    switch (goodKind) {
        case 1:
            toAdd = shared_ptr<Good>(new House(sellerRef));
            break;
        case 2:
            toAdd = shared_ptr<Good>(new Flat(sellerRef));
            break;
        case 3:
            toAdd = shared_ptr<Good>(new Ground(sellerRef));
            break;
        case 4:
            toAdd = shared_ptr<Good>(new Professional(sellerRef));
            break;

        default:
            cout << "Mauvaise entrée\n";
            addGood();
            break;
    }
    goods.push_back(toAdd);
    sellers[sellerName]->addGood(toAdd);
    toAdd->show();
    cout << "Bien ajouté\n\n";
    return toAdd;
}

shared_ptr<Seller> Agency::getSellerRef() {
    cout << "Quel est le nom du vendeur ?\n";

    string sellerName;
    getline(cin, sellerName);

    if (sellers.count(sellerName) == 0) {
        cout << "Quel est l'adresse du vendeur ?\n";
        string sellerAddress;
        getline(cin, sellerAddress);
        sellers[sellerName] = make_shared<Seller>(sellerName, sellerAddress);
    }
    return sellers.find(sellerName)->second;
}

shared_ptr<Seller> Agency::getSellerRef(const std::string &sellerName) {
    return sellers.find(sellerName)->second;
}

shared_ptr<Seller> Agency::addSellerFromBuyer(const shared_ptr<Buyer> &seller){
	shared_ptr<Seller> sellerRef = make_shared<Seller>(seller->getName(), seller->getAddress());
	return sellerRef;
}

void Agency::reSell(){
	shared_ptr<Good> ptrGood = Agency::findGood();
	ptrGood->setSold(false);
	cout << "Quelle est le prix de mise en vente?\n";
	double prix;
	cin >> prix;
	ptrGood->setPrice(prix);
}

void Agency::show() const {
    for (const auto &good : goods) {
        if (!good->isSold()) {
            good->show();
            cout << "======================\n";
        }
    }
}

void Agency::show(const list<shared_ptr<Good>> &goodsList) {
    for (const auto &good : goodsList) {
        if (!good->isSold()) {
            good->show();
            cout << "======================\n";
        }
    }
}

shared_ptr<Buyer> Agency::addBuyer() {
    shared_ptr<Buyer> ptrNewBuyer;
    ptrNewBuyer = make_shared<Buyer>();
    buyers[ptrNewBuyer->getName()] = ptrNewBuyer;
    return ptrNewBuyer;
}

void Agency::search() {
    auto goodsList = getGoods();
    search(goodsList);

}

void Agency::search(const std::list<std::shared_ptr<Good>> &goodsList) {
    cout << "Voici les biens" << endl;
    show(goodsList);
    cout << "Que souhaitez vous faire ?" << endl;
    cout << "\t-1. Affiner la recherche" << endl;
    cout << "\t-2. Visiter un bien" << endl;
    cout << "\t-3. Faire une proposition de vente" << endl;
    cout << "\t-0. Quitter la recherche" << endl;

    int option;
    cin >> option;
    cin.ignore();
    double value;
    bool minMax;
    shared_ptr<Buyer> buyer;
    shared_ptr<Seller> seller;
    shared_ptr<Good> good;
    switch (option) {
        case 1:
            cout << "Vous voulez filtrer par :" << endl;
            cout << "\t-1. Prix" << endl;
            cout << "\t-2. Surface" << endl;
            cout << "\t-3. Type" << endl;

            cin >> option;
            cin.ignore();

            switch (option) {
                case 1:
                    cout << "Vous voulez fixer le prix :" << endl;
                    minMax = Utils::minOrMax();
                    if (minMax) {
                        cout << "Quelle est le prix maximum ?" << endl;
                        cin >> value;
                        cin.ignore();
                        search(filterLowerPrice(value, goodsList));
                    } else {
                        cout << "Quelle est le prix minimum ?" << endl;
                        cin >> value;
                        cin.ignore();
                        search(filterGreaterPrice(value, goodsList));
                    }
                    break;
                case 2:
                    cout << "Vous voulez fixer la surface :" << endl;
                    minMax = Utils::minOrMax();
                    if (minMax) {
                        cout << "Quelle est la surface maximum ?" << endl;
                        cin >> value;
                        cin.ignore();
                        search(filterLowerArea(value, goodsList));
                    } else {
                        cout << "Quelle est la surface minimum ?" << endl;
                        cin >> value;
                        cin.ignore();
                        search(filterGreaterArea(value, goodsList));
                    }
                    break;
                case 3:
                    search(filterType(goodsList));
                    break;
                default:
                    cout << "Vous n'avez pas sélectionner de filtre valide." << endl;
                    search(goodsList);
                    break;
            }

            break;
        case 2:
            good = getGoodByID(goodsList);
            buyer = findBuyer();
            if (buyer != nullptr) {
                buyer->visit(good);
            }
            break;
        case 3:
            good = getGoodByID(goodsList);
            buyer = findBuyer();
            if (buyer != nullptr) {
                cout << "Combien proposez vous pour ce bien ?" << endl;
                good->show();
                int amount;
                cin >> amount;
                good->addProposal(buyer, amount);
            }
            break;

        case 0:
            break;
        default:
            cout << "Vous n'avez pas sélectionner de choix valide." << endl;
            cout << "Voulez vous quitter la navigation ?" << endl;
            if (Utils::yesOrNo()) {
                break;
            } else {
                search(goodsList);
            }
            break;
    }

}


void Agency::visit() {
	std::shared_ptr<Buyer> buyerRef = findBuyer();
	std::shared_ptr<Good> goodRef = findGood();
	buyerRef->visit(goodRef);
}

void Agency::save() {
    ofstream goodsFile;
    goodsFile.open("../Data/biens.txt");
    if (goodsFile.is_open()) {
        for (const auto &good : goods) {
            good->save(goodsFile);
            goodsFile << endl;
        }
        goodsFile.close();
    }


    ofstream sellersFile;
    sellersFile.open("../Data/vendeurs.txt");
    if (sellersFile.is_open()) {
        for (const auto &seller : sellers) {
            seller.second->save(sellersFile);
            sellersFile << endl;
        }
        sellersFile.close();
    }

    ofstream buyersFile;
    buyersFile.open("../Data/acheteurs.txt");
    if (buyersFile.is_open()) {
        for (const auto &buyer : buyers) {
            buyer.second->save(buyersFile);
            buyersFile << endl;
        }
        buyersFile.close();
    }

}


void Agency::load() {
    goods.clear();
    buyers.clear();
    sellers.clear();
    string buffer;
    string name;
    string address;
    ifstream sellerFile("../Data/vendeurs.txt");
    if (sellerFile.is_open()) {
        while (getline(sellerFile, name)) {
            getline(sellerFile, address);
            sellers[name] = make_shared<Seller>(name, address);
            sellerFile.ignore();
        }
        sellerFile.close();
    }

    ifstream buyerFile("../Data/acheteurs.txt");
    if (buyerFile.is_open()) {
        while (getline(buyerFile, name)) {
            getline(buyerFile, address);
            while (buffer != "</VisitedGoods>") {
                getline(buyerFile, buffer);
            }
            buyers[name] = make_shared<Buyer>(name, address);
            buyerFile.ignore();
        }
        buyerFile.close();
    }


    string goodType;
    double goodPrice;
    double goodArea;
    string goodAddress;
    string goodSellerName;
    bool goodSold;
    double proposalAmount;

    int resiNbRooms;
    bool resiGarage;

    bool houseGarden;
    bool houseSwim;

    int flatFloor;
    bool flatCave;
    bool flatBalcony;
    int flatNbBuildingFlats;

    bool groundBuildable;

    double proShowcaseSize;
    bool proStoreRoom;

    list<pair<shared_ptr<Buyer>, double>> proposals;

    ifstream goodsFile("../Data/biens.txt");
    if (goodsFile.is_open()) {
        shared_ptr<Good> toAdd;
        while (getline(goodsFile, goodType)) {
            goodsFile >> goodPrice;
            goodsFile >> goodArea;
            goodsFile.ignore();
            getline(goodsFile, goodAddress);
            getline(goodsFile, goodSellerName);
            goodsFile >> goodSold;
            goodsFile.ignore();
            getline(goodsFile, buffer);
            getline(goodsFile, buffer);
            while (buffer != "</Propositions>") {
                name = buffer;
                goodsFile >> proposalAmount;
                goodsFile.ignore();
                proposals.emplace_back(buyers[name], proposalAmount);
                getline(goodsFile, buffer);
            }

            if (goodType == "Maison" || goodType == "Appartement") {
                goodsFile >> resiNbRooms;
                goodsFile >> resiGarage;

                if (goodType == "Maison") {
                    goodsFile >> houseGarden;
                    goodsFile >> houseSwim;
                    toAdd = shared_ptr<Good>(
                            new House(goodPrice, goodAddress, goodArea, getSellerRef(goodSellerName), goodSold,
                                      resiNbRooms,
                                      resiGarage, houseGarden, houseSwim));
                } else {
                    goodsFile >> flatFloor;
                    goodsFile >> flatCave;
                    goodsFile >> flatBalcony;
                    goodsFile >> flatNbBuildingFlats;
                    toAdd = shared_ptr<Good>(
                            new Flat(goodPrice, goodAddress, goodArea, getSellerRef(goodSellerName), goodSold,
                                     resiNbRooms,
                                     resiGarage, flatFloor, flatCave, flatBalcony, flatNbBuildingFlats));
                }

            } else if (goodType == "Terrain") {
                goodsFile >> groundBuildable;
                toAdd = shared_ptr<Good>(
                        new Ground(goodPrice, goodAddress, goodArea, getSellerRef(goodSellerName),
                                   goodSold, groundBuildable));

            } else if (goodType == "Local") {
                goodsFile >> proShowcaseSize;
                goodsFile >> proStoreRoom;
                toAdd = shared_ptr<Good>(
                        new Professional(goodPrice, goodAddress, goodArea, getSellerRef(goodSellerName), goodSold,
                                         proShowcaseSize, proStoreRoom));
            }
            for (const auto &proposal: proposals) {
                toAdd->addProposal(proposal.first, proposal.second);
            }

            goods.push_back(toAdd);
            sellers[goodSellerName]->addGood(toAdd);
            getline(goodsFile, buffer);
            getline(goodsFile, buffer);
        }
        goodsFile.close();
    }

    buyerFile.open("../Data/acheteurs.txt");
    if (buyerFile.is_open()) {
        while (getline(buyerFile, name)) {
            getline(buyerFile, address);

            getline(buyerFile, buffer);
            getline(buyerFile, buffer);
            while (buffer != "</VisitedGoods>") {
                goodAddress = buffer;
                buyerFile >> goodPrice;
                buyerFile >> goodArea;
                buyerFile.ignore();
                getline(buyerFile, goodSellerName);
                buyers[name]->visit(getGood(goodPrice, goodArea, goodAddress, goodSellerName));
                getline(buyerFile, buffer);
            }
            buyerFile.ignore();
        }
        buyerFile.close();
    }
}

void Agency::sell(){
	shared_ptr<Buyer> buyerRef = Agency::findBuyer();
	shared_ptr<Good> goodRef = Agency::findGood();
	shared_ptr<Seller> sellerRef = Agency::findSeller();
	goodRef->setSold(true);
	shared_ptr<Seller> newSellerRef = Agency::addSellerFromBuyer(buyerRef);



}


shared_ptr<Buyer> Agency::findBuyer() {
// Vérification si l'acheteur existe, récupération de l adresse de l'objet ou proposition de création de l'objet buyer
    cout << "Quel est le nom de l'acheteur?\n";
    string nomAcheteur;
    getline(cin, nomAcheteur); // vérifier que ca autorise les espaces si on décide d entré nom prénom
    auto trouve = buyers.find(nomAcheteur);

    if (trouve == buyers.end()) {
        cout << "L'acheteur n'est pas enregistré dans l'agence\n";
        cout << "Voulez-vous ajouter cette acheteur?\n";
        if (Utils::yesOrNo()) {
            cout << "Quelle est l'adresse de " << nomAcheteur << " ?" << endl;
            string address;
            getline(cin, address);
            buyers[nomAcheteur] = make_shared<Buyer>(nomAcheteur, address);
            return buyers[nomAcheteur];
        } else {
            return nullptr;
        }
    } else {
        return trouve->second;
    }
}

std::shared_ptr<Good>
Agency::getGood(double price, double area, const std::string &address, const std::string &sellerName) {
    for (auto goodPtr: goods) {
        if (goodPtr->getPrice() == price
            && goodPtr->getArea() == area
            && goodPtr->getAddress() == address
            && goodPtr->getSellerName() == sellerName) {
            return goodPtr;
        }
    }
    return nullptr;
}

/**
 * Vérification si le vendeur existe, récupération de l adresse de l'objet ou proposition de création de l'objet seller
 * @return
 */
shared_ptr<Seller> Agency::findSeller() {
    cout << "Quel est le nom de l'acheteur?\n";
    string nomVendeur;
    auto trouve = sellers.find(nomVendeur);

    if (trouve == sellers.end()) {
        cout << "Le vendeur n'est pas enregistré dans l'agence\n";
        return nullptr;
    } else {
        return trouve->second;
    }

}

/**
 * Recherche d'un bien via son adresse
 * @return
 */
shared_ptr<Good> Agency::findGood() {
    cout << "Quelle est l'adresse du bien?\n";
    string adresse;
    getline(cin, adresse);
    auto itGood = goods.begin();
    while (itGood != goods.end()) {
        if ((*itGood)->getAddress() == adresse) {
            return *itGood;
        } else {
            itGood++;
        }
    }
    cout << "L'adresse ne correspond à aucun bien\n";
    return nullptr;
}

/**
 * Mettre des possibilité de sortie de boucle meme si on en trouve pas d acheteur ou de bien
 * 1. trouver le bien
 * 2. trouver l acheteur
 * 3. demander le prix
 * 4. ajouter la proposition d achat dans la map de proposition du bien
 * */
void Agency::addProposal() {
    shared_ptr<Good> ptrBien = nullptr;
    while (ptrBien == nullptr) {
        cout << "Souhaitez-vous rechercher un bien?\n";
        if (Utils::yesOrNo()) {
            ptrBien = findGood();
        } else {
            cout << "Vous n'avez pas sélectionné de bien\n";
            return;
        }
    }

    shared_ptr<Buyer> ptrAcheteur = nullptr;
    while (ptrAcheteur == nullptr) {
        cout << "Souhaitez-vous rechercher un Acheteur?\n";
        if (Utils::yesOrNo()) {
            ptrAcheteur = Agency::findBuyer();
            cout << "Quel est le prix proposé par l'acheteur?\n";
            double prix;
            cin >> prix;
            cin.ignore();
            ptrBien->addProposal(ptrAcheteur, prix);
            cout << "Proposition enregistée\n"; // ajouter une methode show pour les propositions
            ptrBien->showProposals();
        } else {
            cout << "Vous n'avez pas sélectionné de bien\n";
            break;
        }
    }

}

std::list<std::shared_ptr<Good>>
Agency::filterGreaterPrice(double priceThreshold, std::list<std::shared_ptr<Good>> goodsList) {
    std::list<std::shared_ptr<Good>> filteredGoodsList;
    copy_if(goodsList.begin(), goodsList.end(), back_inserter(filteredGoodsList), [priceThreshold](auto val) {
        return val->getPrice() >= priceThreshold && !val->isSold();
    });
    return filteredGoodsList;
}

std::list<std::shared_ptr<Good>>
Agency::filterLowerPrice(double priceThreshold, std::list<std::shared_ptr<Good>> goodsList) {
    std::list<std::shared_ptr<Good>> filteredGoodsList;
    copy_if(goodsList.begin(), goodsList.end(), back_inserter(filteredGoodsList), [priceThreshold](auto val) {
        return val->getPrice() <= priceThreshold && !val->isSold();
    });
    return filteredGoodsList;
}

std::list<std::shared_ptr<Good>>
Agency::filterGreaterArea(double areaThreshold, std::list<std::shared_ptr<Good>> goodsList) {
    std::list<std::shared_ptr<Good>> filteredGoodsList;
    copy_if(goodsList.begin(), goodsList.end(), back_inserter(filteredGoodsList), [areaThreshold](auto val) {
        return val->getArea() >= areaThreshold && !val->isSold();
    });
    return filteredGoodsList;
}

std::list<std::shared_ptr<Good>>
Agency::filterLowerArea(double areaThreshold, std::list<std::shared_ptr<Good>> goodsList) {
    std::list<std::shared_ptr<Good>> filteredGoodsList;
    copy_if(goodsList.begin(), goodsList.end(), back_inserter(filteredGoodsList), [areaThreshold](auto val) {
        return val->getArea() <= areaThreshold && !val->isSold();
    });
    return filteredGoodsList;
}

std::list<std::shared_ptr<Good>> Agency::filterType(std::list<std::shared_ptr<Good>> goodsList) {

    int goodKind = Utils::selectType();

    std::list<std::shared_ptr<Good>> filteredGoodsList;

    switch (goodKind) {
        case 1:
            copy_if(goodsList.begin(), goodsList.end(), back_inserter(filteredGoodsList), [](auto val) {
                return val->getType() == "Maison" && !val->isSold();
            });
            break;
        case 2:
            copy_if(goodsList.begin(), goodsList.end(), back_inserter(filteredGoodsList), [](auto val) {
                return val->getType() == "Appartement" && !val->isSold();
            });
            break;
        case 3:
            copy_if(goodsList.begin(), goodsList.end(), back_inserter(filteredGoodsList), [](auto val) {
                return val->getType() == "Terrain" && !val->isSold();
            });
            break;
        case 4:
            copy_if(goodsList.begin(), goodsList.end(), back_inserter(filteredGoodsList), [](auto val) {
                return val->getType() == "Local" && !val->isSold();
            });
            break;

        default:
            cout << "Mauvaise entrée\n";
            goodsList = filterType(goodsList);
            break;
    }

    return filteredGoodsList;
}


std::list<std::shared_ptr<Good>> Agency::getGoods() const {
    return list<shared_ptr<Good>>(goods);
}

Agency::Agency() {
    load();
}

std::shared_ptr<Good> Agency::getGoodByID(const std::list<std::shared_ptr<Good>> &goodsList) {
    cout << "Quel est le numéro du bien que vous voulez sélectionner ?" << endl;
    int id;
    cin >> id;
    cin.ignore();
    for (auto good : goodsList) {
        if (good->getId() == id) {
            return good;
        }
    }

    cout << "Le numéro que vous avez fournis n'est pas disponible dans la liste que nous vous avons présenter." << endl;
    return getGoodByID(goodsList);
}

Agency::~Agency() {
    save();
    goods.clear();
    buyers.clear();
    sellers.clear();
}
