#include "GroundStation.h"
#include <Coordinates.h>
#include <Constants.h>
#include "Time.h"

using namespace Eigen;
using namespace Transform;

#define OBSERVATION_COORDINATES 2
#define DISTRIBUTION_PARAMS	2

GroundStation::GroundStation() : ObservationModel()
{
	// ��������� ������� ������
	observation_vec_size = OBSERVATION_COORDINATES + 1;

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

	Vector2d sputnik_horiz_pos = Fix2Horiz(X.head(VEC_SIZE), cur_geographic_pos);

	return sputnik_horiz_pos;
}

void GroundStation::SaveObservation(const VectorXd& X, TYPE t, size_t time_moment)
{
	Vector2d sputnik_horiz_pos = MakeObservation(X, t);

	// �������� �� ��������� �������� � ����� ��������� ���
	if (sputnik_horiz_pos(0) >= _vision_zone_angle)
	{
		if (is_session_initialized == false)
			InitObservationSession(time_moment);

		if (do_random)
		{
			sputnik_horiz_pos(0) += distribution(generator, random_error_params[0]);
			sputnik_horiz_pos(1) += distribution(generator, random_error_params[1]);
		}

		ObservationModel::SaveObservation(sputnik_horiz_pos, t, time_moment);
	}
	else if (is_session_initialized == true)
	{
		CloseObservationSession(time_moment);	// ������ ����� ���������� ���������
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






#define DELTA	30	// � ������

GroundStation2::GroundStation2() : GroundStation()
{
	observation_vec_size = 1 + 1;
}

GroundStation2::GroundStation2(const Vector3d &geographic_pos, TYPE vision_zone_angle)
	: GroundStation(geographic_pos, vision_zone_angle)
{
	observation_vec_size = 1 + 1;
}



VectorXd GroundStation2::MakeObservation(const VectorXd & X, TYPE t) const
{
	Vector3d cur_geographic_pos = _geographic_pos;
	cur_geographic_pos(2) += StarTime(start_star_time, t);

	TYPE JD = J2000 + t / SECINDAY;

	Vector3d cur_fix_pos = Geographic2Fix(cur_geographic_pos, JD);
	Vector3d temp = X.head(VEC_SIZE) - cur_fix_pos;
	VectorXd result(1);
	result << temp.norm();
	return result;
}

void GroundStation2::SaveObservation(const VectorXd & X, TYPE t, size_t time_moment)
{
	Vector2d sputnik_horiz_pos = GroundStation::MakeObservation(X, t);

	// �������� �� ��������� �������� � ����� ��������� ���
	if (sputnik_horiz_pos(0) >= _vision_zone_angle)
	{
		if (is_session_initialized == false)
			InitObservationSession(time_moment);

		VectorXd distance = MakeObservation(X, t);

		if (do_random)
		{
			// TODO: ������ �������� � ��
			distance(0) += 
				(DELTA + distribution(generator, random_error_params[0])) / 1000;
		}

		ObservationModel::SaveObservation(distance, t, time_moment);
	}
	else if (is_session_initialized == true)
	{
		CloseObservationSession(time_moment);	// ������ ����� ���������� ���������
	}
}