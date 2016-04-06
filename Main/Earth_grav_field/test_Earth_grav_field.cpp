#include "file_output.h"
#include "Integrator.h"
#include "Time.h"		// для макросов времени
#include "Sputnik.h"

int main()
{
	CDormanPrince Integrator;
	Integrator.setEps_Max(1.0e-13);

	Orbit::Kepler_elements elements = {0, 0, 0, 500, 0, 0};

	CSputnik sputnik(elements);

	sputnik.set_t1(SECINDAY);
	sputnik.setInterval(600);

	Integrator.Run(sputnik);

	to_file(sputnik.getResult());
}