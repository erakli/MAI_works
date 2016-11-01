#include "WhiteNoise.h"

#include "Types.h"
#include "Constants.h"

#include <random>	// ��� ��������� ������ ����

using namespace std;
using namespace Eigen;

/*
*	��������� ������� (�����)������ ���� �� �������� ������� �����
*/
VectorXd GetWhiteNoise(TYPE omega, TYPE t0, TYPE t1, TYPE &dt)
{
	dt = 2 * PI / omega;		// �������� ���������� (�����)������ ����
	TYPE Disp = 1 / dt;			// ��� �� ���������

	VectorXd WhiteNoise;
	size_t vector_size = int((t1 - t0) / dt) + 3;

	WhiteNoise.resize(vector_size);

	// ��������� � ����������� ���������� �������������� ��
	default_random_engine generator;
	normal_distribution<TYPE> distribution(0, sqrt(Disp));
	TYPE temp;

	for (size_t i = 0; i < vector_size; i++)
		WhiteNoise(i) = distribution(generator);

	return WhiteNoise;
}
