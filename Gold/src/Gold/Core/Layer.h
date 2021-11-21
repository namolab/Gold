#pragma once

#include "Gold/Core/Base.h"
#include "Gold/Core/Timestep.h"
#include "Gold/Events/Event.h"


// z���� �������� �ؿ� �򸮰Եȴ�. ����Ƽ������ z�� ����϶� ������ �������� ���⼱ z�� �����϶� ������ ���Ѵ�. 

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