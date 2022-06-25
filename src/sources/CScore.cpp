#include <CScore.hpp>

CScore::CScore(int * num, const std::shared_ptr<CRenderWindow::CText> & textSource)
: score(num),
  text(textSource)
{}

void CScore::operator += (const int & num)
{
    (*this->score) += num;
}

void CScore::render(const std::pair<int,int> & position) const
{
    this->text->render(std::to_string(*this->score), position);
}