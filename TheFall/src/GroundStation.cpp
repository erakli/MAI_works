#include "GroundStation.h"
#include <Coordinates.h>

using namespace Eigen;
using namespace Transform;

#define OBSERVATION_COORDINATES 2
#define DISTRIBUTION_PARAMS	2

GroundStation::GroundStation() : ObservationModel()
{
	// ��������� ������� ������
	observation_vec_size = OBSERVATION_COORDINATES + 1;

	random_error_params.resize(OBSERVATION_COORDINATES, DISTRIBUTION_PARAMS);
	// TODO: �������� ���������� ������� ����������

	// ����������� ���������
	_geographic_pos = Vector3d::Zero();
	_vision_zone_angle = 0.0;

	start_star_time = 0.0;
}

GroundStation::GroundStation(const Vector3d& geographic_pos, TYPE vision_zone_angle)
	: GroundStation()
{
	_geographic_pos = geographic_pos;
	_vision_zone_angle = vision_zone_angle;
}



VectorXd GroundStation::MakeObservation(const VectorXd& X, TYPE t) const
{
	Vector3d cur_geographic_pos = _geographic_pos;
	cur_geographic_pos(2) += StarTime(start_star_time, t);
	Vector2d sputnik_horiz_pos = Fix2Horiz(X, cur_geographic_pos);

	return sputnik_horiz_pos;
}

void GroundStation::SaveObservation(const VectorXd& X, TYPE t)
{
	Vector2d sputnik_horiz_pos = MakeObservation(X, t);

	// �������� �� ��������� �������� � ����� ��������� ���
	if (sputnik_horiz_pos(0) >= _vision_zone_angle)
	{
		ObservationModel::SaveObservation(sputnik_horiz_pos, t);
	}
}



Vector3d GroundStation::GetGeographicPos() const
{
	return _geographic_pos;
}

void GroundStation::SetGeographicPos(const VectorXd& new_geographic_pos)
{
	_geographic_pos = new_geographic_pos;
}



TYPE GroundStation::GetVisionZoneAngle() const
{
	return _vision_zone_angle;
}

void GroundStation::SetVisionZoneAngle(TYPE new_vision_zone_angle)
{
	_vision_zone_angle = new_vision_zone_angle;
}
