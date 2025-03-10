#include "Bingo1.h"
#include <iomanip>

void BingoCard::printCard() const {
    cout << " B   I   N   G   O " << endl;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (card[i][j] == 0) 
                cout << " *   ";  // Espaço livre no centro
            else if (marked[i][j]) 
                cout << "[X]  ";  // Número marcado
            else 
                cout << setw(2) << card[i][j] << "   ";  // Número normal formatado
        }
        cout << endl;
    }
    cout << "---------------------" << endl;
}

BingoCard::BingoCard(set<int>& availableNumbers) : card(5, vector<int>(5)), marked(5, vector<bool>(5, false)) {
    vector<int> columnNumbers;
    
    for (int col = 0; col < 5; ++col) {
        columnNumbers.clear();
        for (int i = 0; i < 15; ++i) {
            int num = col * 15 + i + 1;
            if (availableNumbers.count(num)) columnNumbers.push_back(num);
        }
        shuffle(columnNumbers.begin(), columnNumbers.end(), default_random_engine(random_device{}()));

        for (int row = 0; row < 5; ++row) {
            if (row == 2 && col == 2) {
                card[row][col] = 0; // Espaço livre
                marked[row][col] = true;
            } else {
                card[row][col] = columnNumbers[row];
                availableNumbers.erase(columnNumbers[row]); // Remove para evitar repetição entre cartões
            }
        }
    }
}

void BingoCard::markNumber(int num) {
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            if (card[i][j] == num) marked[i][j] = true;
}

bool BingoCard::checkBingo() const {
    for (int i = 0; i < 5; ++i)
        if (all_of(marked[i].begin(), marked[i].end(), [](bool v) { return v; })) return true;

    for (int i = 0; i < 5; ++i) {
        bool columnBingo = true;
        for (int j = 0; j < 5; ++j)
            if (!marked[j][i]) columnBingo = false;
        if (columnBingo) return true;
    }

    bool diagonal1 = true, diagonal2 = true;
    for (int i = 0; i < 5; ++i) {
        if (!marked[i][i]) diagonal1 = false;
        if (!marked[i][4 - i]) diagonal2 = false;
    }

    return diagonal1 || diagonal2;
}

void BingoCard::printCard() const {
    cout << "B  I  N  G  O\n";
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (card[i][j] == 0) 
                cout << " *  ";  // Espaço livre no centro
            else if (marked[i][j]) 
                cout << "[X] ";  // Número marcado
            else 
                cout << (card[i][j] < 10 ? " " : "") << card[i][j] << "  ";  // Número normal
        }
        cout << endl;
    }
    cout << endl;
}

BingoGame::BingoGame() {
    for (int i = 1; i <= 75; ++i) numberPool.push_back(i);
    shuffle(numberPool.begin(), numberPool.end(), default_random_engine(random_device{}()));

    set<int> availableNumbers(numberPool.begin(), numberPool.end());
    for (int i = 0; i < 3; ++i)
        cards.emplace_back(availableNumbers);
}

void BingoGame::drawNumber() {
    if (numberPool.empty()) {
        cout << "Todos os números já foram sorteados!" << endl;
        return;
    }

    int num = numberPool.back();
    numberPool.pop_back();
    drawnNumbers.insert(num);
    cout << "\n=============================" << endl;
    cout << " Número sorteado: " << num << endl;
    cout << "=============================" << endl;

    for (size_t i = 0; i < cards.size(); ++i) {
        cout << "\nCartão " << i + 1 << ":\n";
        cards[i].markNumber(num);
        cards[i].printCard();
        if (cards[i].checkBingo()) {
            cout << "\n*** BINGO no Cartão " << i + 1 << "! ***" << endl;
            exit(0);
        }
    }
}

void BingoGame::playGame() {
    char escolha;
    while (true) {
        cout << "\n========== MENU ==========" << endl;
        cout << "1 - Sortear número" << endl;
        cout << "2 - Ver cartões" << endl;
        cout << "3 - Sair" << endl;
        cout << "==========================" << endl;
        cout << "Escolha uma opção: ";
        cin >> escolha;

        switch (escolha) {
            case '1':
                drawNumber();
                break;
            case '2':
                for (size_t i = 0; i < cards.size(); ++i) {
                    cout << "\nCartão " << i + 1 << ":\n";
                    cards[i].printCard();
                }
                break;
            case '3':
                cout << "Jogo encerrado." << endl;
                return;
            default:
                cout << "Opção inválida! Escolha 1, 2 ou 3." << endl;
        }
    }
}

//melhoria nos menus, biblioteca nova para melhor formatacao e organizacao
