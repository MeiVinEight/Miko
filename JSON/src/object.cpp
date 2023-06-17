#include <exception.h>
#include <json.h>

JSON::object::object(const JSON::object &copy): content(new JSON::object::KV[copy.size]), size(copy.size), type(copy.type)
{
	for (QWORD i = 0; i < copy.size; i++)
	{
		this->content[i][0] = nullptr;
		switch (copy.type)
		{
			case JSON::type::UNKNOWN:
			{
				this->content[i][1] = nullptr;
				break;
			}
			case JSON::type::STRING:
			{
				this->content[i][1] = new String::string(*((String::string *) copy.content[i][1]));
				break;
			}
			case JSON::type::INTEGER:
			case JSON::type::FLOAT:
			case JSON::type::BOOLEAN:
			{
				this->content[i][1] = copy.content[i][1];
				break;
			}
			case JSON::type::COMPONENT:
			{
				this->content[i][0] = new String::string(*((String::string *) copy.content[i][0]));
				this->content[i][1] = new JSON::object(*((JSON::object *) copy.content[i][1]));
				break;
			}
			case JSON::type::ARRAY:
			{
				this->content[i][1] = new JSON::object(*((JSON::object *) copy.content[i][1]));
				break;
			}
		}
	}
}
JSON::object::object(JSON::object &&move) noexcept: content(move.content), size(move.size), type(move.type)
{
	move.content = nullptr;
	move.size = 0;
	move.type = JSON::type::UNKNOWN;
}
JSON::object::~object()
{
	this->cleanup();
}
JSON::object &JSON::object::operator=(const JSON::object &copy)
{
	if (&copy != this)
	{
		(*this) = JSON::object(copy);
	}
	return *this;
}
JSON::object &JSON::object::operator=(JSON::object &&move) noexcept
{
	if (&move != this)
	{
		this->cleanup();
		this->content = move.content;
		this->size = move.size;
		this->type = move.type;
		move.content = nullptr;
		move.size = 0;
		move.type = JSON::type::UNKNOWN;
	}
	return *this;
}
JSON::object &JSON::object::operator=(const char *str)
{
	return (*this) = String::string(str);
}
JSON::object &JSON::object::operator=(const String::string &str)
{
	if (!this->readonly)
	{
		this->cleanup();
		this->content = new JSON::object::KV[1];
		this->content[0][0] = nullptr;
		this->content[0][1] = new String::string(str);
		this->size = 1;
		this->type = JSON::type::STRING;
		return *this;
	}
	throw Memory::exception(Memory::ERRNO_ACCESS_VIOLATION);
}
JSON::object &JSON::object::operator=(QWORD value)
{
	if (!this->readonly)
	{
		this->cleanup();
		this->content = new JSON::object::KV[1];
		this->content[0][0] = nullptr;
		this->content[0][1] = (void *) value;
		this->size = 1;
		this->type = JSON::type::INTEGER;
		return *this;
	}
	throw Memory::exception(Memory::ERRNO_ACCESS_VIOLATION);
}
JSON::object &JSON::object::operator=(int value)
{
	(void) value;
	return (*this) = (QWORD) value;
}
JSON::object &JSON::object::operator=(double value)
{
	if (!this->readonly)
	{
		QWORD qword = *((QWORD *) &value);
		this->cleanup();
		this->content = new JSON::object::KV[1];
		this->content[0][0] = nullptr;
		this->content[0][1] = (void *) qword;
		this->size = 1;
		this->type = JSON::type::FLOAT;
		return *this;
	}
	throw Memory::exception(Memory::ERRNO_ACCESS_VIOLATION);
}
JSON::object &JSON::object::operator=(bool val)
{
	if (!this->readonly)
	{
		QWORD qv = val;
		this->cleanup();
		this->content = new JSON::object::KV[1];
		this->content[0][0] = nullptr;
		this->content[0][1] = (void *) qv;
		this->size = 1;
		this->type = JSON::type::BOOLEAN;
		return *this;
	}
	throw Memory::exception(Memory::ERRNO_ACCESS_VIOLATION);
}
JSON::object &JSON::object::operator[](const String::string &key)
{
	if (this->type == JSON::type::COMPONENT || this->type == JSON::type::UNKNOWN)
	{
		if (this->readonly && this->type == JSON::UNKNOWN)
			throw Memory::exception(Memory::ERRNO_ACCESS_VIOLATION);

		this->type = JSON::type::COMPONENT;
		QWORD idx = -1;

		for (QWORD i = 0; i < this->size; i++)
		{
			if (*((String::string *) this->content[i][0]) == key)
			{
				idx = i;
				break;
			}
		}

		if (idx == -1)
		{
			if (this->readonly)
				throw Memory::exception(JSON::ERRNO_OBJECT_NOT_FOUND);
			idx = size;
			JSON::object::KV *nkv = new JSON::object::KV[this->size + 1];
			for (int i = 0; i < this->size; i++)
			{
				nkv[i][0] = this->content[i][0];
				nkv[i][1] = this->content[i][1];
			}
			nkv[this->size][0] = new String::string(key);
			nkv[this->size][1] = new JSON::object;
			delete[] this->content;
			this->content = nkv;
			this->size++;
		}

		JSON::object *obj = (JSON::object *) this->content[idx][1];
		obj->readonly = this->readonly;
		return *obj;
	}
	throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);
}
JSON::object &JSON::object::operator[](const char *key)
{
	return (*this)[String::string(key)];
}
JSON::object &JSON::object::operator[](QWORD idx)
{
	if (this->type == JSON::type::ARRAY || this->type == JSON::type::UNKNOWN)
	{
		if (this->readonly && this->type == JSON::type::UNKNOWN)
			throw Memory::exception(Memory::ERRNO_ACCESS_VIOLATION);

		this->type = JSON::type::ARRAY;
		if (idx >= this->size)
		{
			if (this->readonly)
				throw Memory::exception(JSON::ERRNO_OBJECT_NOT_FOUND);

			JSON::object::KV *nkv = new JSON::object::KV[idx + 1];
			for (QWORD i = 0; i < idx + 1; i++)
			{
				nkv[i][0] = nullptr;
				if (i < this->size)
				{
					nkv[i][1] = this->content[i][1];
				}
				else
				{
					nkv[i][1] = new JSON::object;
				}
			}
			delete[] this->content;
			this->content = nkv;
			this->size = idx + 1;
		}

		JSON::object *obj = (JSON::object *) this->content[idx][1];
		obj->readonly = this->readonly;
		return *obj;
	}
	throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);
}
JSON::object &JSON::object::operator[](int idx)
{
	return this->operator[]((QWORD) idx);
}
JSON::object::operator String::string &() const
{
	if (this->type == JSON::type::STRING)
	{
		return *((String::string *) this->content[0][1]);
	}
	throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);
}
JSON::object::operator QWORD() const
{
	if (this->type == JSON::type::INTEGER)
	{
		return (QWORD) this->content[0][1];
	}
	throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);
}
JSON::object::operator int() const
{
	return (int)(QWORD)(*this);
}
JSON::object::operator double() const
{
	if (this->type == JSON::type::FLOAT)
	{
		return *((double *) &this->content[0][1]);
	}
	throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);
}
JSON::object::operator bool() const
{
	if (this->type == JSON::type::BOOLEAN)
	{
		return (bool) (*((QWORD *) &this->content[0][1]));
	}
	throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);
}
QWORD JSON::object::length() const
{
	return this->size;
}
void JSON::object::cleanup()
{
	switch (this->type)
	{
		case JSON::type::STRING:
		{
			delete ((String::string *) this->content[0][1]);
			break;
		}
		case JSON::type::COMPONENT:
		{
			for (QWORD i = 0; i < this->size; i++)
			{
				delete ((String::string *) this->content[i][0]);
				delete ((JSON::object *) this->content[i][1]);
			}
			break;
		}
		case JSON::type::ARRAY:
		{
			for (QWORD i = 0; i < this->size; delete ((JSON::object *) this->content[i++][1]));
			break;
		}
	}
	delete[] this->content;
	this->content = nullptr;
	this->size = 0;
	this->type = JSON::type::UNKNOWN;
}
bool JSON::object::contain(const String::string &key) const
{
	if (this->type != JSON::type::COMPONENT)
		throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);

	for (QWORD i = 0; i < this->size; i++)
	{
		if (*((String::string *) this->content[i][0]) == key)
		{
			return true;
		}
	}
	return false;
}
bool JSON::object::contain(const char *key) const
{
	return this->contain(String::string(key));
}
String::string JSON::object::stringify() const
{
	switch (this->type)
	{
		case JSON::type::UNKNOWN:
		{
			return "null";
		}
		case JSON::type::STRING:
		{
			return JSON::stringify((String::string &) *this);
		}
		case JSON::type::INTEGER:
		{
			return String::stringify(*this, true);
		}
		case JSON::type::FLOAT:
		{
			return String::stringify((double) *this);
		}
		case JSON::type::BOOLEAN:
		{
			return ((bool) *this) ? "true" : "false";
		}
		case JSON::type::COMPONENT:
		{
			String::string str = "{";
			bool dot = false;
			for (QWORD i = 0; i < this->size; i++)
			{
				if (dot)
					str += ",";
				str += JSON::stringify(*((String::string *) this->content[i][0]));
				str += ":";
				str += (*((JSON::object *) this->content[i][1])).stringify();
				dot = true;
			}
			str += "}";
			return str;
		}
		case JSON::type::ARRAY:
		{
			String::string str = "[";
			bool dot = false;
			for (QWORD i = 0; i < this->size; i++)
			{
				if (dot)
					str += ",";

				str += (*((JSON::object *) this->content[i][1])).stringify();
				dot = true;
			}
			str += "]";
			return str;
		}
	}
	throw Memory::exception(JSON::ERRNO_WRONG_OBJECT_TYPE);
}