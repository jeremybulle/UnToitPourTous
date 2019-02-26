//
// Created by eliot on 25/02/19.
//

#include <iostream>
#include <limits>
#include "../../Headers/Goods/Good.h"
#include "../../Headers/Clients/Seller.h"

using namespace std;

int Good::nbInstance = 0;

Good::Good(const Good &src) : price(src.price),
                              address(src.address), area(src.area),
                              sellerRef(src.sellerRef){
    id = ++nbInstance;
}

Good::Good(Seller *sellerRef) : sellerRef(sellerRef) {
    cout << "Quelle est l'adresse du bien ?\n";
    getline(cin, address);
    cout << "Quelle est le prix du bien ?\n";
    cin >> price;
    cout << "Quelle est la surface du bien ?\n";
    cin >> area;
    cin.ignore();
    id = ++nbInstance;
}

Good::Good(double price, const string &address, double area, Seller *sellerRef) : price(price), address(address),
                                                                                  area(area), sellerRef(sellerRef) {
    id = ++nbInstance;
}

Seller *Good::getSellerRef() const {
    return sellerRef;
}

void Good::show() const {
    cout <<
    "\t-Prix : " << price << "€\n" <<
    "\t-Surface : " << area << "m²\n" <<
    "\t-Adresse : " << address << "\n" <<
    "\t-Nom du vendeur : " << sellerRef->getName() << "\n";
}

