cbuffer CPUData
{
	matrix transformationsMatrix;
}
struct VShaderOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};
