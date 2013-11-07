#ifndef CPP_DB_XSQLDA_H
#define CPP_DB_XSQLDA_H

#include "ibase.h"

#include <string>

namespace cpp_db
{

class xsqlda
{
public:
	xsqlda();
	~xsqlda();

	XSQLDA *get() const;

	int get_var_count() const;
	bool resize_to_fit();
	void init();

	XSQLVAR &operator[](int index);
	const XSQLVAR &operator[](int index) const;

	std::string get_field_name(int index) const;

private:
	void allocate(int vars_count);
	void release();

private:
	XSQLDA *sqlda;
};

}

#endif
