// ParticleSystemHW1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
//#include "ParticleSystem.h"
//#include "ParticleSystemHW1.h"
#include <iostream>
#include <assert.h>

using namespace std;

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:
	/*
	ParticleSystem() : ParticleSystem(10*4){
		m_vertices = new sf::VertexArray(sf::Quads, 10 * 4);
	}
	*/
	ParticleSystem(unsigned int count) :
		m_particles(count),
		m_vertices(),
		//m_vertices(sf::PrimitiveType, count),
		//m_vertices(sf::Quads, count),
		//m_vertices(sf::Quads, count*4),
		//m_vertices(sf::TrianglesFan, count),
		//m_vertices(sf::TrianglesStrip, count),	/*Works*/
		//m_vertices(sf::Triangles, count),		/*Works*/
		//m_vertices(sf::LinesStrip, count),	/*Works*/
		//m_vertices(sf::Lines, count),			/*Works*/
		//m_vertices(sf::Points, count),
		m_lifetime(sf::seconds(3)),
		m_emitter(0, 0),
		m_texture(),
		m_gravity(100.0f, 100.0f)
	{
		//m_vertices(sf::Quads, count);
		if (count % 2 == 0) 
		{
			m_vertices.setPrimitiveType(sf::Quads);
			m_vertices.resize(count);
		}
		else
		{
			m_vertices.setPrimitiveType(sf::Points);
			m_vertices.resize(count);
		}
		//m_vertices.resize(count);
		if (!m_texture.loadFromFile("RainbowWaterColor.gif"))
			cout << "Image was not loaded!";
	}
	/*
	ParticleSystem(unsigned int count) :
		m_particles(count),
		m_vertices(sf::Points, count),
		m_lifetime(sf::seconds(3)),
		m_emitter(0, 0),
		m_texture(),
		m_gravity(100.0f, 100.0f)
	{
		if (!m_texture.loadFromFile("RainbowWaterColor.gif"))
			cout << "Image was not loaded!";
	}
	*/
	void setEmitter(sf::Vector2f position)
	{
		//m_emitter = position;
		if (m_vertices.getPrimitiveType() == sf::Points)
		{
			setEmitterPoints(position);
			//assert (false);
		}
		else if (m_vertices.getPrimitiveType() == sf::Quads)
		{
			setEmitterQuads(position);
		}
		else
		{
			assert(false);
		}
	}

	void update(sf::Time elapsed)
	{
		const sf::Time &t = elapsed;
		if (m_vertices.getPrimitiveType() == sf::Points)
		{
			update_points(t);
			//assert (false);
		}
		else if (m_vertices.getPrimitiveType() == sf::Quads)
		{
			update_quads(t);
		}
		else
		{
			assert(false);
		}
	}

	
	void addParticle()
	{
		if (m_vertices.getPrimitiveType() == sf::Points)
		{
			addPoint();
			//assert(false);
		}
		else if (m_vertices.getPrimitiveType() == sf::Quads)
		{
			addQuad();
		}
		else
		{
			assert(false);
		}
	}
	
	//*/
private:
	void setEmitterPoints(sf::Vector2f position)
	{
		m_emitter = position;
	}

	void setEmitterQuads(sf::Vector2f position)
	{
		m_emitter = position;
	}
	void update_quads(sf::Time elapsed)
	{
		for (std::size_t i = 0; i < m_particles.size(); i += 4)
		{
			// update the particle lifetime
			Particle& p = m_particles[i];
			Particle& q = m_particles[i + 1];
			Particle& r = m_particles[i + 2];
			Particle& s = m_particles[i + 3];

			const sf::Time &t = elapsed;
			//const float t_check = elapsed.asSeconds;

			//Insure const t does not change throughout this iteration
			//assert(t.asSeconds == t_check);

			p.lifetime -= t;
			q.lifetime -= t;
			r.lifetime -= t;
			s.lifetime -= t;

			// if any vertex of the quad particle is dead, respawn the quad particle
			if (p.lifetime <= sf::Time::Zero || q.lifetime <= sf::Time::Zero || r.lifetime <= sf::Time::Zero || s.lifetime <= sf::Time::Zero)
				resetParticle(i);

			// update the position of the corresponding vertex
			//p.velocity.x += m_gravity.x * elapsed.asSeconds();
			p.velocity.y += m_gravity.y * t.asSeconds();
			q.velocity.y += m_gravity.y * t.asSeconds();
			r.velocity.y += m_gravity.y * t.asSeconds();
			s.velocity.y += m_gravity.y * t.asSeconds();

			//m_vertices[i].position += p.velocity * elapsed.asSeconds();
			m_vertices[i].position += p.velocity * t.asSeconds();
			m_vertices[i + 1].position += q.velocity * t.asSeconds();
			m_vertices[i + 2].position += r.velocity * t.asSeconds();
			m_vertices[i + 3].position += p.velocity * t.asSeconds();

			// update the alpha (transparency) of the particle according to its lifetime
			const float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
			const float ratio_check = ratio;

			assert(ratio == ratio_check);

			m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
			m_vertices[i + 1].color.a = static_cast<sf::Uint8>(ratio * 255);
			m_vertices[i + 2].color.a = static_cast<sf::Uint8>(ratio * 255);
			m_vertices[i + 3].color.a = static_cast<sf::Uint8>(ratio * 255);
		}
	}

	void update_points(sf::Time elapsed)
	{
		for (std::size_t i = 0; i < m_particles.size(); ++i)
		{
			// update the particle lifetime
			Particle& p = m_particles[i];
			p.lifetime -= elapsed;

			// if the particle is dead, respawn it
			if (p.lifetime <= sf::Time::Zero)
				resetParticle(i);

			// update the position of the corresponding vertex
			p.velocity.y += m_gravity.y * elapsed.asSeconds();
			m_vertices[i].position += p.velocity * elapsed.asSeconds();

			// update the alpha (transparency) of the particle according to its lifetime
			float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
			m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
		}
	}

	void addQuad()
	{
		//Declare particle and push it onto the particles vector
		Particle *p = new Particle;
		sf::Vertex *v = new sf::Vertex;

		for (int i = 0; i < 4; i++) {
			m_particles.push_back(*p);
			m_vertices.append(*v);
			//Reset the newly added particle to have random properties	
		}
		resetParticle(m_particles.size() - 4);
		//resetParticle(m_particles.size() - 3);
		//resetParticle(m_particles.size() - 2);
		//resetParticle(m_particles.size() - 1);
	}
	///*
	void addPoint()
	{
		//Declare particle and push it onto the particles vector
		Particle *p = new Particle;
		sf::Vertex *v = new sf::Vertex;

		for (int i = 0; i < 100; i++) {
			m_particles.push_back(*p);
			m_vertices.append(*v);

			resetParticle(m_particles.size() - 1);
		}
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_vertices.getPrimitiveType() == sf::Points)
		{
			drawPoint(target, states);
			//assert(false);
		}
		else if (m_vertices.getPrimitiveType() == sf::Quads)
		{
			drawQuad(target, states);
		}
		else
		{
			assert(false);
		}
	}
	virtual void drawQuad(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();
		
		// use a texture
		states.texture = &m_texture;
		/*
		// our particles don't use a texture
		states.texture = NULL;
		*/
		// draw the vertex array
		target.draw(m_vertices, states);
	}
	///*
	virtual void drawPoint(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		//states.texture = &m_texture;
		// our particles don't use a texture
		states.texture = NULL;

		// draw the vertex array
		target.draw(m_vertices, states);
	}
	//*/
private:

	struct Particle
	{
		sf::Vector2f velocity;
		sf::Time lifetime;
	};
	/*
	struct Repeller
	{
		float strength = 100.0f;
		sf::Vector2f location;
		float r = 10.0f;
	};
	*/
	void resetParticle(std::size_t index)
	{
		if (m_vertices.getPrimitiveType() == sf::Points)
		{
			resetPoint(index);
			//assert(false);
		}
		else if (m_vertices.getPrimitiveType() == sf::Quads)
		{
			resetQuad(index);
		}
		else
		{
			assert(false);
		}
	}

	void resetQuad(std::size_t index)
	{
		// give a random velocity and lifetime to the particle
		//float angle = (std::rand() % 6) * 45.f;
		//float angle = (std::rand() % 6) * 60.f;
		float angle = (std::rand() % 3) * 90.f;
		//float angle = (std::rand() % 360) * 3.14f / 180.f;		
		
		float speed = (std::rand() % 50) + 50.f;

		//Insure computed angle and speed stay constant for each vertex of the quad relative to each other
		const float m_angle = angle;
		const float m_speed = speed;

		//Update velocity for each vertex of the quadrelative to each other
		//m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
		m_particles[index].velocity = sf::Vector2f(std::cos(m_angle) * m_speed, std::sin(m_angle) * m_speed);
		m_particles[index+1].velocity = sf::Vector2f(std::cos(m_angle) * m_speed, std::sin(m_angle) * m_speed);
		m_particles[index+2].velocity = sf::Vector2f(std::cos(m_angle) * m_speed, std::sin(m_angle) * m_speed);
		m_particles[index+3].velocity = sf::Vector2f(std::cos(m_angle) * m_speed, std::sin(m_angle) * m_speed);

		//Insure lifetime stays constant for each vertex of the quad
		const sf::Time t = sf::milliseconds((std::rand() % 2000) + 1000);

		//Update lifetime for each vertex of the quad
		m_particles[index].lifetime = t;
		m_particles[index+1].lifetime = t;
		m_particles[index+2].lifetime = t;
		m_particles[index+3].lifetime = t;

		// reset the position of the corresponding vertex
		//m_vertices[index].position = m_emitter;

		// define it as a rectangle, located at m_emitter and with size 100x100
		m_vertices[index].position.x	= m_emitter.x;
		m_vertices[index].position.y	= m_emitter.y;
		m_vertices[index+1].position.x	= m_emitter.x + square_width;
		m_vertices[index+1].position.y	= m_emitter.y;
		m_vertices[index+2].position.x	= m_emitter.x + square_width;
		m_vertices[index+2].position.y	= m_emitter.y + square_height;
		m_vertices[index+3].position.x	= m_emitter.x;
		m_vertices[index+3].position.y	= m_emitter.y + square_height;

		// define its texture area to be a 25x50 rectangle starting at m_emitter
		m_vertices[index].texCoords.x		= m_emitter.x;
		m_vertices[index].texCoords.y		= m_emitter.y;
		m_vertices[index+1].texCoords.x		= m_emitter.x + texture_width;
		m_vertices[index+1].texCoords.y		= m_emitter.y;
		m_vertices[index+2].texCoords.x		= m_emitter.x + texture_width;
		m_vertices[index+2].texCoords.y		= m_emitter.y + texture_height;
		m_vertices[index+3].texCoords.x		= m_emitter.x;
		m_vertices[index+3].texCoords.y		= m_emitter.y + texture_height;
	}
	///*
	void resetPoint(std::size_t index)
	{
		// give a random velocity and lifetime to the particle
		float angle = (std::rand() % 360) * 3.14f / 180.f;
		//float angle = (std::rand() % 6) * 45.f;
		float speed = (std::rand() % 50) + 50.f;
		m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
		m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

		// reset the position of the corresponding vertex
		m_vertices[index].position = m_emitter;
	}
	//*/
	std::vector<Particle> m_particles;
	sf::VertexArray m_vertices;
	sf::Time m_lifetime;
	sf::Vector2f m_emitter;
	
	sf::Texture m_texture;
	sf::Vector2f m_gravity;	//Example given here = https://github.com/SFML/SFML/wiki/Source%3A-Particle-System
	const int square_width = 100/5;
	const int square_height = 100/5;
	const int texture_width = 25/5;
	const int texture_height = 50/5;
};

int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(512, 256), "Particles");

	// create the particle system
	ParticleSystem particles(251);
	ParticleSystem quads(10*4);
	sf::Vector2f coord(200.0f, 100.0f);
	
	// create a clock to track the elapsed time
	sf::Clock clock;

	// run the main loop
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			///*
			if (event.type == sf::Event::KeyPressed)
			{
					particles.addParticle();
					quads.addParticle();
			}
			//*/
			/*
			if (event.type == sf::Event::KeyPressed)
			{
				quads.addParticle();
			}
			*/
			/*
			if (event.type == sf::Event::MouseWheelMoved)
			{
				particles.addParticle();
				//quads.addParticle(true);
			}
			*/
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// make the particle system emitter follow the mouse
		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		//particles.setEmitter(window.mapPixelToCoords(mouse));	

		particles.setEmitter(coord);

		quads.setEmitter(window.mapPixelToCoords(mouse));

		// update it
		sf::Time elapsed = clock.restart();
		particles.update(elapsed);
		quads.update(elapsed);

		// draw it
		window.clear();
		window.draw(particles);
		window.draw(quads);
		window.display();
	}

	return 0;
}

//Class taken from the book "Nature of Code" Ch 4.12
class Repeller {
	sf::Vector2f location;
	float r = 10;
	float G = 100;

	Repeller(float x, float y)
	{
		location.x = x;
		location.y = y;
	}

	void display()
	{
		//stroke(255);
		//fill(255);
		//ellipse(location.x, location.y, r * 2, r * 2);
	}

	float constrain(float amt, float min, float max)
	{
		if (amt < min)
			return min;
		else if (amt > max)
			return max;
		else
			return amt;
	}

	sf::Vector2f repel(sf::Vector2f p)
	{
		// Calculate direction of force
		//PVector dir = PVector.sub(location, p.location);  <-- In the Processing PL
		float dir_x = fabs(location.x - p.x);
		float dir_y = fabs(location.y - p.y);
		float dir_mag = sqrt(dir_x*dir_x + dir_y*dir_y);
		float dir_x_normal = dir_x / dir_mag;
		float dir_y_normal = dir_y / dir_mag;

		dir_mag = constrain(dir_mag, 5, 100);
		float force = -1 * G / (dir_mag * dir_mag);
		float dx = force * dir_x_normal;
		float dy = force * dir_y_normal;
		sf::Vector2f dir(dx, dy);
		
		return dir;
	}
};


