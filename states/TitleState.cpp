#include <cmath>
#include "TitleState.h"

TitleState::TitleState(StateStack& stack, Context context)
: State(stack, context)
, mText()
, mShowText(true)
, mTextEffectTime(sf::Time::Zero)
{
    //mBackgroundSprite.setTexture(context.textures->getResource(Textures::TitleScreen));
    mText.setFont(context.fonts->getResource(Fonts::MainNumbers));
    mText.setString("Press any key to start");
    sf::FloatRect bounds = mText.getLocalBounds();
    mText.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    mText.setPosition(context.window->getView().getSize() / 2.f);
    mTitles.setFont(context.fonts->getResource(Fonts::MainLetters));
    mTitles.setString("25050! Shlyapa gaming\nPresents: CUBHEAD");
    bounds = mTitles.getLocalBounds();
    mTitles.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    mTitles.setPosition(context.window->getView().getSize().x/2, context.window->getView().getSize().y/3);
}

void TitleState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);
    window.draw(mTitles);
    if (mShowText)
        window.draw(mText);
}

bool TitleState::update(sf::Time dt)
{
    mTextEffectTime += dt;

    if (mTextEffectTime >= sf::seconds(0.5f))
    {
        mShowText = !mShowText;
        mTextEffectTime = sf::Time::Zero;
    }

    return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
    // If any key is pressed, trigger the next screen
    if (event.type == sf::Event::KeyPressed)
    {
        requestStackPop();
        requestStackPush(States::Menu);
    }

    return true;
}