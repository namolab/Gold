#pragma once

#include "Gold/Core/Base.h"
#include "Gold/Core/Timestep.h"
#include "Gold/Events/Event.h"


// z값이 작을수록 밑에 깔리게된다. 유니티에서의 z가 양수일때 앞으로 향하지만 여기선 z가 음수일때 앞으로 향한다. 

namespace Gold {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}