#pragma once

#include "Types.h"
#include "LinearAlgebra.h"


class Model
{
protected:
	Eigen::VectorXd StartValues;

	size_t
		s_size; // длина вектора StartValues

	TYPE 
		Interval, // должен ли быть интервал между знач. задан тут?
		t0, 
		t1;

	Eigen::MatrixXd Result;	// двунаправленный список
	size_t last_result_idx;
	size_t result_line_length;

	// Остановка интегрирования при малых изменениях приращения координаты
	TYPE stop_condition;
	unsigned short int stop_count, stop_count_max;
	bool stop_flag;

public:
	Model();
	Model(const Model& other);
	virtual ~Model();

	Model& operator=(const Model& right);

public:
	virtual void Init(int argc = 0, void** argv = nullptr);	// инициализация модели

	virtual Eigen::VectorXd getRight(const Eigen::VectorXd &X, TYPE t) const = 0;

	virtual bool Stop_Calculation(
		TYPE t, TYPE Step, const Eigen::VectorXd &PrevStep, const Eigen::VectorXd &CurStep);

public:
	virtual void addResult(const Eigen::VectorXd &X, TYPE t);
	Eigen::MatrixXd getResult() const;
	Eigen::VectorXd getLastResult() const;
	void clearResult();

	Eigen::VectorXd getStart() const;
	void setStart(const Eigen::VectorXd &arg);

	TYPE getInterval() const;
	void setInterval(TYPE arg);

	TYPE get_t0() const;
	void set_t0(TYPE arg);

	TYPE get_t1() const;
	void set_t1(TYPE arg);	

protected:
	virtual void SetStartValuesSize(size_t new_size, size_t result_line_expansion_by = 1);
	virtual void SetStartValuesSize(const Eigen::VectorXd& new_start_values, size_t result_line_expansion_by = 1);
	void SetResultSize();
};