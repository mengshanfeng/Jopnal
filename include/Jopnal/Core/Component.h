// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

#ifndef JOP_COMPONENT_HPP // Comment - Include guard format: JOP_FILENAME_FILEEXTENSION
#define JOP_COMPONENT_HPP

// Headers
//#include <Jopnal/Core/Object.hpp> 
#include <iostream>
//////////////////////////////////////////////

namespace jop
{
	class Component
	{
	public:
		virtual ~Component();//virtual destructor

		virtual void SendMessage(std::string message);//Send messages

		virtual void Update(float deltaTime);
		virtual void Init(void);
		virtual void Uninit(void);

		bool isActive(void) const;//Check if active - false means it will be deleted

	private:
		int compID = 0; // each component type has unique ID
	};

	class TestComponent : public Component
	{
	public:
		TestComponent();
		//base functions for component
		bool IsWorking();
		int GetValues(){ return x; };
	private:
		int x = 2;
		
	};
}




#endif