#include "Dictionary.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

Question::Question(int num, const std::string &text) : number(num), questionText(text) {}

int Question::getNumber() const
{
    return number;
}

std::string Question::getQuestionText() const
{
    return questionText;
}

SingleAnswerQuestion::SingleAnswerQuestion(int num, const std::string &text, const std::string &a, const std::string &b, const std::string &c, char correct)
    : Question(num, text), optionA(a), optionB(b), optionC(c), correctAnswer(correct) {}

void SingleAnswerQuestion::viewQuestion() const
{
    std::cout << "Pytannia #" << number << ": " << questionText << "\n";
    std::cout << "a) " << optionA << "\n";
    std::cout << "b) " << optionB << "\n";
    std::cout << "v) " << optionC << "\n";
    std::cout << "Vvedit pravyl'nu vidpovid (a, b abo v): ";
}

bool SingleAnswerQuestion::checkAnswer(const std::string &userAnswer) const
{
    return userAnswer.length() == 1 && userAnswer[0] == correctAnswer;
}

char SingleAnswerQuestion::getCorrectAnswer() const
{
    return correctAnswer;
}

MultipleChoiceQuestion::MultipleChoiceQuestion(int num, const std::string &text, const std::string &a, const std::string &b, const std::string &c, const std::set<char> &correct)
    : Question(num, text), answerA(a), answerB(b), answerC(c), correctAnswers(correct) {}

void MultipleChoiceQuestion::viewQuestion() const
{
    std::cout << "Pytannia #" << number << ": " << questionText << "\n";
    std::cout << "a) " << answerA << "\n";
    std::cout << "b) " << answerB << "\n";
    std::cout << "v) " << answerC << "\n";
    std::cout << "Vvedit vsi pravyl'ni vidpovidi (napryklad, 'ab', 'bv' abo 'abv'): ";
}

bool MultipleChoiceQuestion::checkAnswer(const std::string &userAnswer) const
{
    std::set<char> userAnswers(userAnswer.begin(), userAnswer.end());
    return userAnswers == correctAnswers;
}

std::string MultipleChoiceQuestion::getAnswerA() const
{
    return answerA;
}

std::string MultipleChoiceQuestion::getAnswerB() const
{
    return answerB;
}

std::string MultipleChoiceQuestion::getAnswerC() const
{
    return answerC;
}

std::set<char> MultipleChoiceQuestion::getCorrectAnswers() const
{
    return correctAnswers;
}

Dictionary::~Dictionary()
{
    for (auto question : questions)
    {
        delete question;
    }
}

void Dictionary::addSingleAnswerQuestion(int number, const std::string &questionText, const std::string &a, const std::string &b, const std::string &c, char correctAnswer)
{
    questions.push_back(new SingleAnswerQuestion(number, questionText, a, b, c, correctAnswer));
}

void Dictionary::addMultipleChoiceQuestion(int number, const std::string &questionText, const std::string &a, const std::string &b, const std::string &c, const std::set<char> &correctAnswers)
{
    questions.push_back(new MultipleChoiceQuestion(number, questionText, a, b, c, correctAnswers));
}

void Dictionary::deleteQuestion(int number)
{
    auto it = std::remove_if(questions.begin(), questions.end(), [number](Question *q)
                             { return q->getNumber() == number; });
    if (it != questions.end())
    {
        delete *it;
        questions.erase(it);
        std::cout << "Pytannia #" << number << " bylo vydaleno.\n";
    }
    else
    {
        std::cout << "Pytannia z takim nomerom ne znaideno.\n";
    }
}

void Dictionary::viewQuestions() const
{
    if (questions.empty())
    {
        std::cout << "Nemae pytan dlia perehliadu.\n";
        return;
    }

    for (const auto &question : questions)
    {
        question->viewQuestion();
    }
}

void Dictionary::takeTest() const
{
    if (questions.empty())
    {
        std::cout << "Nemae pytan dlia testu.\n";
        return;
    }

    int correctCount = 0;
    for (const auto &question : questions)
    {
        question->viewQuestion();
        std::string userAnswer;
        std::cin >> userAnswer;
        if (question->checkAnswer(userAnswer))
        {
            ++correctCount;
            std::cout << "Pryvyl'no!\n";
        }
        else
        {
            std::cout << "Nepryvil'no!\n";
        }
    }

    std::cout << "Vash rezultat: " << correctCount << " z " << questions.size() << "\n";
}

void Dictionary::saveToFile() const
{
    std::ofstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << "Pomyilka pry vidkrytti fajlu.\n";
        return;
    }

    for (const auto &question : questions)
    {
        if (SingleAnswerQuestion *saq = dynamic_cast<SingleAnswerQuestion *>(question))
        {
            file << "SingleAnswer\n";
            file << saq->getNumber() << "\n";
            file << saq->getQuestionText() << "\n";
            file << saq->getAnswerA() << "\n";
            file << saq->getAnswerB() << "\n";
            file << saq->getAnswerC() << "\n";
            file << saq->getCorrectAnswer() << "\n";
        }
        else if (MultipleChoiceQuestion *mcq = dynamic_cast<MultipleChoiceQuestion *>(question))
        {
            file << "MultipleChoice\n";
            file << mcq->getNumber() << "\n";
            file << mcq->getQuestionText() << "\n";
            file << mcq->getAnswerA() << "\n";
            file << mcq->getAnswerB() << "\n";
            file << mcq->getAnswerC() << "\n";
            for (char answer : mcq->getCorrectAnswers())
            {
                file << answer;
            }
            file << "\n";
        }
    }
    file.close();
}

void Dictionary::loadFromFile()
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << "Ne vdalosya vidkryty fajl dlia chytannia.\n";
        return;
    }

    std::string type;
    while (file >> type)
    {
        if (type == "SingleAnswer")
        {
            int number;
            std::string questionText, optionA, optionB, optionC;
            char correctAnswer;

            file >> number;
            file.ignore();
            std::getline(file, questionText);
            std::getline(file, optionA);
            std::getline(file, optionB);
            std::getline(file, optionC);
            file >> correctAnswer;

            addSingleAnswerQuestion(number, questionText, optionA, optionB, optionC, correctAnswer);
        }
        else if (type == "MultipleChoice")
        {
            int number;
            std::string questionText, a, b, c;
            std::set<char> correctAnswers;
            std::string correct;

            file >> number;
            file.ignore();
            std::getline(file, questionText);
            std::getline(file, a);
            std::getline(file, b);
            std::getline(file, c);
            std::getline(file, correct);

            for (char ch : correct)
            {
                correctAnswers.insert(ch);
            }

            addMultipleChoiceQuestion(number, questionText, a, b, c, correctAnswers);
        }
    }
    file.close();
}
