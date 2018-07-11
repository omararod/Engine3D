#include "DataDefinitions.hlsli"


VShaderOutput main(float4 position : POSITION, float4 color : COLOR)
{
	VShaderOutput output;

	output.position =  mul( transformationsMatrix,position);
	output.color = color ;
    return output;
}