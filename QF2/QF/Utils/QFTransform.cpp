#include "../QF.h"

const std::string QF::Utils::Transform::g_StringFromStringVector(
	const std::vector<std::string>& _Vec, const std::string& _LineEnding)
{ 
	const size_t _VecFinal = (_Vec.size() - 1);
	if (_VecFinal == -1) return __QF_EMPTY_STRING;

	std::string _Buffer; 

	/* Reserve some memory */
	_Buffer.reserve(_Buffer.size() * 100);

	for (size_t _Iterator = 0; _Iterator <= _VecFinal;
		_Iterator++)
	{ /* Add to buffer, if last line dont add ending */
		std::string _Ending = (_Iterator != _VecFinal ? _LineEnding : __QF_EMPTY_STRING);

		(_Buffer += std::format("{}{}", _Vec[_Iterator], _Ending));
	}
	return _Buffer;	 
}

const std::string QF::Utils::Transform::g_WithoutOccurences(
	const std::string& _Str, const std::string& _Occurence)
{
	std::string _StrCopy = _Str;

	size_t _Pos; 
	/* Remove all */
	while ((_Pos = _StrCopy.find(_Occurence)) 
		!= std::string::npos)
	{ /* Erase */
		_StrCopy.erase(_Pos, _Occurence.length());
	}
	/* Return modified copy */
	return _StrCopy;
}

const std::vector<float> QF::Utils::Transform::g_VectorFloatFromImColor(const ImColor& _Color)
{
	std::vector<float> _Values; 
	_Values.push_back(_Color.Value.x * 255);
	_Values.push_back(_Color.Value.y * 255);
	_Values.push_back(_Color.Value.z * 255);
	_Values.push_back(_Color.Value.w * 255);

	return _Values;
}

const ImColor QF::Utils::Transform::g_ImColorFromFloatVector(const std::vector<float>& _Values)
{
	return ImColor(static_cast<int>(_Values[0]), static_cast<int>(_Values[1]), static_cast<int>(_Values[2]), static_cast<int>(_Values[3]));
}