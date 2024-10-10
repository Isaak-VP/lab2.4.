#include <iostream>
#include "Dictionary.hpp"
#include <set>

void menu(Dictionary &dict) {
    int choice;
    do {
        std::cout << "Meniu:\n";
        std::cout << "1. Dodaty zapytannia z odnieiu vidpoviddiu\n";
        std::cout << "2. Dodaty zapytannia z kilkoma variantamy\n";
        std::cout << "3. Vydalyty zapytannia\n";
        std::cout << "4. Perehliad usikh zapytannia\n";
        std::cout << "5. Proity test\n";
        std::cout << "6. SAVE\n";

        std::cout << "Vyberit optsiu: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            int number;
            std::string questionText;
            char correctAnswer;
            std::string optionA, optionB, optionC;

            std::cout << "Vvedit nomer pytannia: ";
            std::cin >> number;
            std::cin.ignore();
            std::cout << "Vvedit pytannia: ";
            std::getline(std::cin, questionText);

            std::cout << "Vvedit variant vidpovidi a: ";
            std::getline(std::cin, optionA);
            std::cout << "Vvedit variant vidpovidi b: ";
            std::getline(std::cin, optionB);
            std::cout << "Vvedit variant vidpovidi v: ";
            std::getline(std::cin, optionC);

            std::cout << "Vvedit pravyl'nu vidpovid (a, b abo v): ";
            std::cin >> correctAnswer;

            dict.addSingleAnswerQuestion(number, questionText, optionA, optionB, optionC, correctAnswer);
            break;
        }
        case 2: {
            int number;
            std::string questionText, a, b, c;
            std::set<char> correctAnswers;
            std::cout << "Vvedit nomer pytannia: ";
            std::cin >> number;
            std::cin.ignore();
            std::cout << "Vvedit pytannia: ";
            std::getline(std::cin, questionText);
            std::cout << "Vvedit variant vidpovidi a: ";
            std::getline(std::cin, a);
            std::cout << "Vvedit variant vidpovidi b: ";
            std::getline(std::cin, b);
            std::cout << "Vvedit variant vidpovidi v: ";
            std::getline(std::cin, c);
            std::cout << "Vvedit pravyl'nu vidpovid (a, b, v abo kombinatsiiu): ";
            std::string correct;
            std::cin >> correct;

            for (char ch : correct) {
                correctAnswers.insert(ch);
            }

            dict.addMultipleChoiceQuestion(number, questionText, a, b, c, correctAnswers);
            break;
        }
        case 3: {
            int number;
            std::cout << "Vvedit nomer pytannia dlia vydalennia: ";
            std::cin >> number;
            dict.deleteQuestion(number);
            break;
        }
        case 4:
            dict.viewQuestions();
            break;
        case 5:
            dict.takeTest();
            break;
        case 6:
            dict.saveToFile();
            break;
        default:
            std::cout << "Nevirny vybir.\n";
        }
    } while (choice != 6);
}

int main() {
    Dictionary dict;
    dict.loadFromFile();  
    menu(dict);
    return 0;
}
