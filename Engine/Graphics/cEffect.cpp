// Includes
//=========

#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>

#include <new>


eae6320::cResult eae6320::Graphics::cEffect::Create(std::shared_ptr<cEffect>& o_effect, const std::string& i_vertexShaderPath, const std::string& i_fragmentShaderPath)
{
	auto result = eae6320::Results::Success;

	std::shared_ptr<cEffect> newEffect;

	// If a new effect instance is successfully created, pass it out from 
	// the function. Otherwise, clean up the instance.
	eae6320::cScopeGuard scopeGuard([&o_effect, &result, &newEffect]
		{
			if (result)
			{
				EAE6320_ASSERT(newEffect != nullptr);
				o_effect.swap(newEffect);
			}
			else
			{
				if (newEffect != nullptr)
				{
					newEffect.reset();
				}
				o_effect = nullptr;
			}
		});

	// Allocate a new effect
	{
		newEffect = std::shared_ptr<cEffect>(new (std::nothrow) cEffect(), [](cEffect* const i_effect) { delete i_effect; });
		if (!newEffect)
		{
			result = eae6320::Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the effect");
			eae6320::Logging::OutputError("Failed to allocate memory for the effect");
			return result;
		}
	}
	// Initialize the platform-specific graphics API effect object
	{
		if (!(result = newEffect->Initialize(i_vertexShaderPath, i_fragmentShaderPath)))
		{
			EAE6320_ASSERTF(false, "Initialization of new effect failed");
			return result;
		}
	}

	return result;
}


eae6320::Graphics::cEffect::~cEffect()
{
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}


eae6320::cResult eae6320::Graphics::cEffect::InitializeShader(const std::string& i_vertexShaderPath, const std::string& i_fragmentShaderPath)
{
	auto result = eae6320::Results::Success;

	if (!(result = eae6320::Graphics::cShader::Load(
		i_vertexShaderPath, m_vertexShader, eae6320::Graphics::eShaderType::Vertex)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without vertex shader");
		return result;
	}
	if (!(result = eae6320::Graphics::cShader::Load(
		i_fragmentShaderPath, m_fragmentShader, eae6320::Graphics::eShaderType::Fragment)))
	{
		EAE6320_ASSERTF(false, "Can't initialize shading data without fragment shader");
		return result;
	}
	{
		m_renderState = new cRenderState();

		constexpr auto renderStateBits = []
		{
			uint8_t renderStateBits = 0;

			eae6320::Graphics::RenderStates::DisableAlphaTransparency(renderStateBits);
			//eae6320::Graphics::RenderStates::DisableDepthTesting(renderStateBits);
			//eae6320::Graphics::RenderStates::DisableDepthWriting(renderStateBits);
			eae6320::Graphics::RenderStates::EnableDepthTesting(renderStateBits);
			eae6320::Graphics::RenderStates::EnableDepthWriting(renderStateBits);
			eae6320::Graphics::RenderStates::DisableDrawingBothTriangleSides(renderStateBits);

			return renderStateBits;
		}();
		if (!(result = m_renderState->Initialize(renderStateBits)))
		{
			EAE6320_ASSERTF(false, "Can't initialize shading data without render state");
			return result;
		}
	}

	return result;
}


eae6320::cResult eae6320::Graphics::cEffect::CleanUpShader()
{
	if (m_vertexShader)
	{
		m_vertexShader->DecrementReferenceCount();
		m_vertexShader = nullptr;
	}
	if (m_fragmentShader)
	{
		m_fragmentShader->DecrementReferenceCount();
		m_fragmentShader = nullptr;
	}
	{
		delete m_renderState;
		m_renderState = nullptr;
	}

	return eae6320::Results::Success;
}

