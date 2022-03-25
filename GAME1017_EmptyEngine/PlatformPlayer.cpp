#include "PlatformPlayer.h"
#include "EventManager.h"
#include "TextureManager.h"
#include <cmath>
#include "States.h"

PlatformPlayer::PlatformPlayer(SDL_Rect s, SDL_FRect d) : AnimatedSpriteObject(s, d),

	m_state(STATE_JUMPING), m_isGrounded(false), m_isFacingLeft(false),
		m_maxVelX(9.0), m_maxVelY(JUMPFORCE), m_grav(GRAVITY), m_drag(0.85) 


{
		m_accelX = m_accelY = m_velX = m_velY = 0.0;
	}


void PlatformPlayer::Update()
{
	switch (m_state) { //inside each case is the behaving in and transiton from that state
	case STATE_IDLING:
		//transitoin to run
		if (EVMA::KeyPressed(SDL_SCANCODE_A) || EVMA::KeyPressed(SDL_SCANCODE_D)) {
			m_state = STATE_RUNNING;
			//SetAnimation(?,?,?,?);
			SetAnimation(288, 480, 64, 64);
		}
		//transition to jump
		else if ( EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_isGrounded) {

			m_accelY = -JUMPFORCE;
			m_isGrounded = false;
			m_state = STATE_JUMPING;
			//set animation (?,?,?,?);
			SetAnimation(288, 480, 64, 64);
		}


		break;
	case STATE_RUNNING:

		//move on ground
		if (EVMA::KeyHeld(SDL_SCANCODE_A)) {
			m_accelX = -1.5;
			if (!m_isFacingLeft)
				m_isFacingLeft = true;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D)) {
			m_accelX = 1.5;
			if (m_isFacingLeft)
				m_isFacingLeft = false;
		}



		//transition to idle
		if (!EVMA::KeyHeld(SDL_SCANCODE_A) && !EVMA::KeyHeld(SDL_SCANCODE_D)) {

			m_state = STATE_IDLING;

		}

		// 
		// 
		//transitiont to jump
		else if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_isGrounded) {

			m_accelY = -JUMPFORCE;
			m_isGrounded = false;
			m_state = STATE_JUMPING;
			//set animation (?,?,?,?);
			SetAnimation(288, 480, 64, 64);
		}


		break;
	case STATE_JUMPING:

		if (EVMA::KeyHeld(SDL_SCANCODE_A)){

			m_accelX = -1.5;
			if (!m_isFacingLeft)
				m_isFacingLeft = true;

		}
		else if(EVMA::KeyHeld(SDL_SCANCODE_D)) {

			m_accelX = 1.5;
			if (m_isFacingLeft)
				m_isFacingLeft = false;
		}

		if (m_isGrounded) {
			m_state = STATE_RUNNING;


			//setanimation(?,?,?,?);
			SetAnimation(288, 480, 64, 64);



		}

		break;
	}


	m_velX += m_accelX;
	m_velX *= (m_isGrounded ? m_drag : 1.0);	
	m_velX = std::min(std::max(m_velX, -m_maxVelX), m_maxVelX);
	m_dst.x += (float)m_velX;

	//yaxis
	m_velY += m_accelY + m_grav;
	m_velY = std::min(std::max(m_velY, -m_maxVelY), m_maxVelY);
	m_dst.y += (float)m_velY;

	m_accelX = m_accelY = 0.0;
	//animate();
}

void PlatformPlayer::Render()
{
	//to animate the sprite, use sdl_rendercopyexf() and you wll ahve to access the texture manager and pass in a key such as "player".

//	SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("player"), &m_src, &m_dst, m_sprite, nullptr, SDL_FLIP_NONE);

	SDL_RenderCopyF(Engine::Instance().GetRenderer(), TEMA::GetTexture("player"), &m_src, &m_dst);

	

	// 
	// 
	// 
	//but for this part, w'erd going to use a colored square


	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 255);
	SDL_RenderFillRectF(Engine::Instance().GetRenderer(), &m_dst);

	
}

void PlatformPlayer::Stop()
{
	StopX();
	StopY();
}

void PlatformPlayer::StopX()
{
	m_velX = 0.0;
}

void PlatformPlayer::StopY()
{
	m_velY = 0.0;
}

void PlatformPlayer::SetAccelX(double a)
{
	m_accelX = a;
}

void PlatformPlayer::SetAccelY(double a)
{
	m_accelY = a;
}

void PlatformPlayer::SetX(float x)
{
	m_dst.x = x;
}

void PlatformPlayer::SetY(float y)
{
	m_dst.y = y;
}

void PlatformPlayer::SetGrounded(bool g)
{
	m_isGrounded = g;

}

bool PlatformPlayer::IsGrounded()
{
	return m_isGrounded;
}

double PlatformPlayer::GetVelX()
{
	return m_velX;
}

double PlatformPlayer::GetVelY()
{
	return m_velY;
}
