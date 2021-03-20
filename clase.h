#pragma once
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
class save_error
{
public:
	char* mes = nullptr;
	save_error() {}

	save_error(char* mes)
	{
		this->mes = new char[strlen(mes) + 1];
		strcpy_s(this->mes, strlen(mes) + 1, mes);
	}
	~save_error() {  }
};
class collumn {

	string name = " ";
	string type = " ";

	float default_value_float;
	string default_value = " ";

	vector<float> valori_float;
	vector<string>  valori_char;

	int dimensiune = 0;

	int nr_rows = 0;
public:
	collumn()
	{

	}
	collumn(string name, string type, string default_value, string dimensiune, string nr_rows = "default")
	{
		this->name = name;
		this->type = type;
		this->dimensiune = stoi(dimensiune);
		if (this->type == "int" || this->type == "INT" || this->type == "float" || this->type == "FLOAT") {
			if (!default_value.empty()) default_value_float = stoi(default_value);
			else default_value_float = 0;
		}
		else {
			if (!default_value.empty()) this->default_value = default_value;
			else this->default_value = "0";
		}
		if (nr_rows != "default") this->nr_rows = stoi(nr_rows);
		else this->nr_rows = 0;
	}


	void decide(string& word, int mod = 0)
	{
		if (!mod) this->nr_rows++;
		if (this->type == "int" || this->type == "INT" || this->type == "float" || this->type == "FLOAT")
		{
			if (word.find(" ") != -1) valori_float.push_back(default_value_float);
			else valori_float.push_back(stoi(word));
		}
		else {
			if (word.find(" ") != -1) valori_char.push_back(default_value);
			valori_char.push_back(word);
		}
	}
	int search_row(int* start, string value)
	{
		while (*start < this->nr_rows)
		{
			if ((this->type == "char" || this->type == "CHAR") && this->valori_char[*start] == value) return *start;
			else if (this->valori_float[*start] == stoi(value)) return *start;
			(*start)++;
		}

		return -1;
	}
	int& get_dim()
	{
		return this->dimensiune;
	}
	string& getname()
	{
		return name;
	}
	string& gettype()
	{
		return type;
	}
	string& get_defvalue()
	{
		return default_value;
	}
	float& get_defvaluefloat()
	{
		return default_value_float;
	}
	vector<float>& get_valori_float()
	{
		return this->valori_float;
	}
	vector<string>& get_valori_char()
	{
		return this->valori_char;
	}
	int& get_nr_rows()
	{
		return nr_rows;
	}
	void push_value(string v)
	{
		this->valori_char.push_back(v);
	}
	void push_value_float(string v)
	{
		this->valori_float.push_back(stoi(v));
	}
};
class table {
	string table_name = "";
	vector<collumn*> coloane;
	int nr_coloane = 0;
	int nr_rows = 0;

public:
	table()
	{
		//
	}
	table(string name, int col, int row)
	{
		cout << "in constructor\n";
		this->table_name = name;
		this->nr_coloane = col;
		this->nr_rows = row;

	}
	table(string& n_com, string& com, int& p)
	{
		com.erase(0, 1);
		cout << "\n" << com << "\n";
		if (n_com.size() == 0 || com.size() == 0) {
			p = 0;
			return;
		}
		else table_name.assign(n_com);

		if (p) {
			unsigned int firstpoz = 0, isOk = 0;
			string cname, type, default_value, dimensiune;

			while (firstpoz != com.size() - 1)
			{


				string str;
				int lastpoz = com.find(')', firstpoz);

				if (lastpoz != -1)str.assign(com.begin() + firstpoz, com.begin() + lastpoz + 1);
				else str.assign(com.begin() + firstpoz, com.end() - 1);


				collumn_parameters(cname, type, default_value, dimensiune, str, isOk, com);

				if (isOk == 3 || isOk == 2) {
					if (prep(cname, type, dimensiune, default_value)) {
						cout << "\nfinal " << cname << " " << type << " " << dimensiune << " *" << default_value << "*\n";
						collumn* p = new collumn;
						collumn cp(cname, type, default_value, dimensiune);
						*p = cp;
						coloane.push_back(p);
						this->nr_coloane++;
					}
					else { p = 0; return; }
				}
				else { p = 0; return; }
				firstpoz = com.find('(', lastpoz + 1);
				if (firstpoz == -1 || lastpoz == -1) firstpoz = com.size() - 1;

			}
		}
	}
	void del_spaces(string& str)
	{
		for (int i = 0; i < str.size(); i++)
			if (!str.empty())
				if (str.at(i) == ' ') str.erase(i, 1);
	}
	int prep(string& name, string& type, string& dim, string& val)
	{
		del_spaces(name);
		del_spaces(type);
		del_spaces(dim);
		del_spaces(val);
		if (type != "integer" && type != "text") return 0;
		if (type == "integer") type = "int";
		if (type == "text") type = "char";

		if (!val.empty()) {
			cout << "\n val not empty\n" << val << "\n";
			if (val.at(0) == '\'' && val.size() == 2) val = "0";
			else
			{
				if (val.at(0) == '\'')
					val.erase(0, 1);
				if (val.at(val.size() - 1) == '\'')
					val.erase(val.size() - 1, 1);
			}
		}
		else val = "0";

		return 1;
	}
	void put_collum(collumn* p)
	{
		this->coloane.emplace_back(p);
	}
	void collumn_parameters(string& name, string& type, string& default_value, string& dimensiune, string& com, unsigned int& isOk, string& ofcom)
	{

		if (com.at(0) == '(') com.erase(0, 1);
		//if (com.at(com.size() - 1) == ')') com.erase(com.size() - 1, 1);
		cout << "\n" << com << " \n";
		isOk = 0;
		for (unsigned int k = 0; k < com.size(); k++)
		{


			if (com.at(k) == ',') isOk++;

		}
		cout << "\n" << isOk << " is ok\n";
		if (isOk == 2 || isOk == 3) {
			int k = 0;

			int j = com.find(',', k + 1); cout << "\n" << j << "\n";
			name.assign(com, k, j - k);

			k = com.find(',', j + 1);   cout << "\n" << k << "\n";
			type.assign(com, j + 1, k - j - 1);

			if (isOk == 2) {
				j = com.find(')', k + 1);
				default_value.assign(com, k + 1, j - k - 1);
				dimensiune = '0';
			}
			else {
				j = com.find(',', k + 1);  cout << "\n" << j << "\n";
				dimensiune.assign(com, k + 1, j - k - 1);

				k = com.find(')', j + 1); cout << "\n" << k << "\n";
				default_value.assign(com, j + 1, k - j - 1);
			}

			cout << name << " " << type << " " << dimensiune << " " << default_value << "\n";




		}
	}
	void adaugare(string& sub, int i_table, int mod = 0)
	{
		int no_words = 0;
		if (!mod) sub.erase(0, 1);
		unsigned int firstpoz = 0;
		while (firstpoz < sub.size())
		{
			int lastpoz = sub.find(',', firstpoz);

			string word;
			if (lastpoz != -1) {
				word.assign(sub.begin() + firstpoz, sub.begin() + lastpoz);
				del_spaces(word);
				cout << "\n *" << word << "*";
				if (!mod) {
					if (word.at(0) == '"') {
						word.erase(0, 1);
						word.erase(word.size() - 1, 1);
					}
				}
				table::tables_pointers[i_table]->get_coloane()[no_words]->decide(word); no_words++;
				firstpoz = lastpoz + 1;

			}
			else {
				if (firstpoz == sub.size() - 1) word = sub.at(firstpoz);
				else {
					if (!mod) word.assign(sub.begin() + firstpoz, sub.begin() + sub.size() - 1);
					else  word.assign(sub.begin() + firstpoz, sub.begin() + sub.size());
				}
				del_spaces(word);
				if (!mod) {
					if (word.at(0) == '"') {
						word.erase(0, 1);
						word.erase(word.size() - 1, 1);
					}
				}
				table::tables_pointers[i_table]->get_coloane()[no_words]->decide(word); no_words++;
				return;
			}
		}
	}
	void set_name(string& name) {
		this->table_name = name;
	}

	int& get_nr_coloane()
	{
		return nr_coloane;
	}
	string& getname()
	{
		return table_name;
	}
	vector<collumn*> get_coloane()
	{
		return coloane;
	}

	void print_row(int counter, string fwhere, int row, int* p = nullptr)
	{
		ofstream file(fwhere, ios::app);
		file << row << " ";
		cout << row << " ";
		if (!p) {

			for (int i = 0; i < this->nr_coloane; i++)
			{
				cout << "|    ";

				if (this->decide2(this->coloane[i])) {
					cout << this->coloane[i]->get_valori_char()[row];
					file << "|";
					print_spaces(file, (20 - this->coloane[i]->get_valori_char()[row].length()) / 2);
					file << this->coloane[i]->get_valori_char()[row];
					if (20 - this->coloane[i]->get_valori_char()[row].length() % 2) print_spaces(file, (20 - this->coloane[i]->get_valori_char()[row].length()) / 2 + 1);
					else print_spaces(file, (20 - this->coloane[i]->get_valori_char()[row].length()) / 2);
				}
				else {
					file << "|";
					cout << this->coloane[i]->get_valori_float()[row];
					print_spaces(file, (20 - to_string(this->coloane[i]->get_valori_float()[row]).length()) / 2);
					file << this->coloane[i]->get_valori_float()[row];
					if (20 - to_string(this->coloane[i]->get_valori_float()[row]).length()) print_spaces(file, (20 - to_string(this->coloane[i]->get_valori_float()[row]).length()) / 2 + 1);
					else print_spaces(file, (20 - to_string(this->coloane[i]->get_valori_float()[row]).length()) / 2);
				}
				cout << "     ";

			}
			cout << "|\n";
			file << "|\n";
		}
		else {
			for (int i = 0; i < this->nr_coloane; i++)
			{
				if (p[i]) {
					cout << "|    ";

					if (this->decide2(this->coloane[i])) {
						cout << this->coloane[i]->get_valori_char()[row];
						file << "|";
						print_spaces(file, (20 - this->coloane[i]->get_valori_char()[row].length()) / 2);
						file << this->coloane[i]->get_valori_char()[row];
						if (30 - this->coloane[i]->get_valori_char()[row].length() % 2) print_spaces(file, (20 - this->coloane[i]->get_valori_char()[row].length()) / 2 + 1);
						else print_spaces(file, (20 - this->coloane[i]->get_valori_char()[row].length()) / 2);
					}
					else {
						file << "|";
						cout << this->coloane[i]->get_valori_float()[row];
						print_spaces(file, (20 - to_string(this->coloane[i]->get_valori_float()[row]).length()) / 2);
						file << this->coloane[i]->get_valori_float()[row];
						if (20 - to_string(this->coloane[i]->get_valori_float()[row]).length()) print_spaces(file, (20 - to_string(this->coloane[i]->get_valori_float()[row]).length()) / 2 + 1);
						else print_spaces(file, (20 - to_string(this->coloane[i]->get_valori_float()[row]).length()) / 2);
					}
					cout << "     ";

				}
			}
			cout << "|\n";
			file << "|\n";
		}
		file.close();
	}
	void print_spaces(ofstream& file, int nr)
	{
		for (int i = 0; i < nr; i++)
			file << " ";
	}
	void print_separator(ofstream& file, int nr)
	{
		for (int i = 0; i < nr; i++)
			file << "-";
	}
	void print(int& c, bool format = 0, int* p = nullptr)
	{
		if (!format) {
			ofstream d("Display/DISPLAY_" + to_string(c) + ".txt");
			cout << "\n";
			cout << "----------------------------------------------------------------";
			print_separator(d, this->nr_coloane * 20);
			cout << "\n";
			d << "\n";
			cout << "   |                      " << this->table_name << "                          |";
			d << "  |";
			print_spaces(d, ((this->nr_coloane * 20) - this->table_name.length()) / 2 - 3);
			d << this->table_name;
			print_spaces(d, ((this->nr_coloane * 20) - this->table_name.length()) / 2);
			d << "|";
			cout << "\n";
			d << "\n";
			cout << "----------------------------------------------------------------";
			print_separator(d, this->nr_coloane * 20);
			cout << "\n";
			d << "\n";
			d << "  ";
			//cout << this->nr_coloane;
			for (int i = 0; i < this->nr_coloane; i++) {
				cout << "  |   " << this->coloane[i]->getname() << "   ";
				d << "|";
				print_spaces(d, (20 - this->coloane[i]->getname().length()) / 2);
				d << this->coloane[i]->getname();
				if (20 - this->coloane[i]->getname().length()) print_spaces(d, (20 - this->coloane[i]->getname().length()) / 2 + 1);
				else print_spaces(d, (20 - this->coloane[i]->getname().length()) / 2);
			}

			cout << "  |\n";
			d << "|\n";
			cout << "----------------------------------------------------------------\n";
			print_separator(d, this->nr_coloane * 20);
			d << "\n";
			d.close();
		}
		else {
			ofstream s("Select/SELECT_" + to_string(c) + ".txt");
			s << "SELECT " << c << " RESULT from " << this->getname() << " :\n";
		}
		int n = this->coloane[0]->get_nr_rows();
		for (int row = 0; row < n; row++)
		{
			if (!format) {
				/*ofstream d("DISPLAY_" + to_string(c) + ".txt", ios::app);
				cout << row << " ";
				d << row << " ";*/
				print_row(c, "Display/DISPLAY_" + to_string(c) + ".txt", row, p);
				/*d.close();*/
			}
			else {
				/*ofstream s("SELECT_" + to_string(c) + ".txt", ios::app);
				cout << row << " ";
				s << row << " ";*/
				print_row(c, "Select/SELECT_" + to_string(c) + ".txt", row, p);
				/*s.close();*/
			}
		}
		cout << "\n";
	}
	int search_collumn(string name)
	{

		for (int i = 0; i < this->nr_coloane; i++)
		{
			if (this->coloane[i]->getname() == name) return i;
		}
		return -1;
	}
	bool decide2(collumn* coloana)
	{
		if (coloana->gettype() == "char" || coloana->gettype() == "CHAR") return 1;
		else return 0;
	}

	void update_row(int i_c1, string v1, int i_c2, string v2)
	{
		collumn* id = this->coloane[i_c1];
		collumn* name = this->coloane[i_c2];
		string v9 = v1;
		string ella = v2;


		for (int i = 0; i < this->coloane[0]->get_nr_rows(); i++)
		{
			if (decide2(id) == 1) {
				if (decide2(name) == 1) {
					if (name->get_valori_char()[i] == ella)
						id->get_valori_char()[i] = v9;
				}
				else if (decide2(name) == 0)
				{
					if (name->get_valori_float()[i] == stoi(ella))
						id->get_valori_char()[i] = v9;
				}
			}
			else if (decide2(id) == 0) {
				if (decide2(name) == 1) {
					if (name->get_valori_char()[i] == ella)
					{
						id->get_valori_float().at(i) = stoi(v9);
					}
				}
				else if (decide2(name) == 0) {
					if (name->get_valori_float()[i] == stoi(ella))
						id->get_valori_float()[i] = stoi(v9);
				}
			}
		}
	}


	static  vector<table*> tables_pointers;
	static int cautare_obiect(string&);

	static bool itExists(string&);


};

vector<table*> table::tables_pointers;
bool table::itExists(string& n_com)
{
	if (table::tables_pointers.empty()) return 0;
	if (table::cautare_obiect(n_com) != -1) return 1;
	return 0;
}
int table::cautare_obiect(string& n_com)
{
	if (table::tables_pointers.empty()) return -1;

	for (unsigned int i = 0; i < table::tables_pointers.size(); i++)
	{

		if (table::tables_pointers[i]->table_name == n_com) {

			return i;
		}
	}
	return -1;
}
class files
{
protected:
	string name = "";
	string conf_data = "";
	string binary_data = "";
	int nr_rows = -1;
	files(){}
	files(int nr) {
		this->name = "Report_" + to_string((int)nr) + ".txt";
		this->conf_data = "";
		this->binary_data = "";
		this->nr_rows = -1;
	}
public:
	files(string& name)
	{
		try {
			int i = table::cautare_obiect(name);
			if (!table::tables_pointers[i]->get_coloane().empty())
				this->nr_rows = table::tables_pointers[i]->get_coloane()[0]->get_nr_rows();
			else this->nr_rows = -1;
			this->conf_data = "Tables/" + name + ".txt";
			this->binary_data = "Tables/" + name + "_data.bin";
			ofstream conf(conf_data);
			if (conf.is_open())
			{
				conf << table::tables_pointers[i]->getname() << endl;
				conf << table::tables_pointers[i]->get_nr_coloane() << " ";
				conf << this->nr_rows << endl;
				for (int j = 0; j < table::tables_pointers[i]->get_nr_coloane(); j++)
				{
					conf << table::tables_pointers[i]->get_coloane()[j]->getname() << " ";
					conf << table::tables_pointers[i]->get_coloane()[j]->gettype() << " ";
					if (table::tables_pointers[i]->decide2(table::tables_pointers[i]->get_coloane()[j]))
						conf << table::tables_pointers[i]->get_coloane()[j]->get_defvalue() << " ";
					else conf << table::tables_pointers[i]->get_coloane()[j]->get_defvaluefloat() << " ";
					conf << table::tables_pointers[i]->get_coloane()[j]->get_dim() << " ";

					for (int k = 0; k < this->nr_rows; k++)
					{
						if (table::tables_pointers[i]->decide2(table::tables_pointers[i]->get_coloane()[j]))
							conf << table::tables_pointers[i]->get_coloane()[j]->get_valori_char()[k].length() << " ";
						else {
							convert_to_string(table::tables_pointers[i]->get_coloane()[j]->get_valori_float()[k], table::tables_pointers[i]->get_coloane()[j]);
							conf << table::tables_pointers[i]->get_coloane()[j]->get_valori_char()[k].length() << " ";
						}
					}
					conf << endl;
				}
				conf.close();
			}
			else {
				char* mes = new char[35]{ "\nConfiguration file not open.\n" };
				throw save_error(mes);
				//delete[] mes;
			}

			if (table::tables_pointers[i]->get_coloane()[0]->get_nr_rows())
			{
				ofstream bin(binary_data, ios::out | ios::binary | ios::trunc);
				if (bin.is_open()) {
					for (int j = 0; j < table::tables_pointers[i]->get_nr_coloane(); j++)
					{
						for (int k = 0; k < table::tables_pointers[i]->get_coloane()[j]->get_nr_rows(); k++) {
							bin.write(table::tables_pointers[i]->get_coloane()[j]->get_valori_char()[k].c_str(), table::tables_pointers[i]->get_coloane()[j]->get_valori_char()[k].length());
							bin.write("\0", sizeof(char));
						}
					}
				}
				else {
					char* mes = new char[30]{ "\nBinary file not open.\n" };
					throw save_error(mes);
					//delete[] mes;
				}
			}
		}
		catch (save_error er)
		{
			cout << er.mes;

		}

	}
	void convert_to_string(float x, collumn* col)
	{

		string value = to_string((int)x);
		col->push_value(value);
	}
};

class report : public files
{
protected:
	string folder_name = "";
public:
	report(string folder,int nr) :files(nr)
	{
		this->folder_name = folder;
	}
	virtual void generate_raport()
	{}
	string& get_name()
	{
		return files::name;
	}
};
class db_report :public report
{
public:
	db_report(string folder, int nr) : report(folder, nr)
	{

	}
	void generate_raport()
	{
		ofstream file(report::folder_name + files::name);
		if (file)
		{
			for (int i = 0; i < table::tables_pointers.size(); i++)
			{
				file << "\n\n---------------------------------------------------------------------------------\n";
				file << "                                 " << table::tables_pointers[i]->getname() << "                \n";
				file << "------------------------------------------------------------------------------------------\n";
				for (int j = 0; j < table::tables_pointers[i]->get_nr_coloane(); j++)
				{
					file << "   ";
					file << table::tables_pointers[i]->get_coloane()[j]->getname();
					file << "   |";
				}
				file << "\n";


				for (int j = 0; j < table::tables_pointers[i]->get_coloane()[0]->get_nr_rows(); j++)
				{
					file << j << " |";
					print_row(file, j, i);
				}


			}
		}
		file.close();
	}
	void print_row(ofstream& file, int nr_row,int table)
	{
		for (int i = 0; i < table::tables_pointers[table]->get_nr_coloane(); i++)
		{
			if (table::tables_pointers[table]->decide2(table::tables_pointers[table]->get_coloane()[i]))
				file << "  " << table::tables_pointers[table]->get_coloane()[i]->get_valori_char()[nr_row] << "   |";
			else file << "   " << table::tables_pointers[table]->get_coloane()[i]->get_valori_float()[nr_row] << "    |";
		}
		file << "\n";
	}
	
};

class table_index {
	string index_name;
	string table_name;
	int t_index;
	string collumn_name;
	int* rows_indexed;
	int nr_rows_indexed;
public:
	table_index()
	{
		this->index_name = "";
		this->table_name = "";
		this->collumn_name = "";
		this->rows_indexed = nullptr;
		this->nr_rows_indexed = 0;
		this->t_index = 0;

	}
	table_index(int i_table, int i_collum, string value, string col_name = "", int start = -1, string name = "", string t_name = "")
	{
		set_name(name);
		set_table_name(t_name);
		set_collum_name(i_table, i_collum, col_name, &start, value);

	}
	~table_index()
	{
		if (this->rows_indexed)
			delete[] this->rows_indexed;
	}
	string get_name()
	{
		return this->index_name;
	}
	int* get_rows_indexed()
	{
		return this->rows_indexed;
	}
	int get_nr_rows_indexed()
	{
		return this->nr_rows_indexed;
	}
	void set_name(string name)
	{
		this->index_name = name;
	}
	void set_table_name(string t_name)
	{
		this->table_name = t_name;
	}
	void set_collum_name(int i_table, int i_collum, string col_name, int* start, string value)
	{
		this->collumn_name = col_name;
		this->t_index = i_table;
		if (rows_indexed) delete[] rows_indexed;
		rows_indexed = new int[table::tables_pointers[i_table]->get_coloane()[0]->get_nr_rows()];
		int j = 0;
		if (*start != -1) {
			rows_indexed[0] = *start;
			j = 1;
		}

		for (int i = *start + 1; i < table::tables_pointers[i_table]->get_coloane()[i_collum]->get_nr_rows(); i++)
			if (table::tables_pointers[i_table]->get_coloane()[i_collum]->gettype() == "char" || table::tables_pointers[i_table]->get_coloane()[i_collum]->gettype() == "CHAR")
			{
				if (table::tables_pointers[i_table]->get_coloane()[i_collum]->get_valori_char()[i] == value) this->rows_indexed[j++] = i;
			}

			else  if (table::tables_pointers[i_table]->get_coloane()[i_collum]->get_valori_float()[i] == stoi(value)) this->rows_indexed[j++] = i;
		this->nr_rows_indexed = j;
	}
	void print_index(int& sc, int* p = nullptr)
	{
		cout << "\n" << this->nr_rows_indexed << "\n";
		//for (int i = 0; i < this->nr_rows_indexed; i++) {
		ofstream file("Select/SELECT_" + to_string(sc) + ".txt");
		if (file) file << "SELECT " << sc << " RESULTS" << this->get_name() << " :\n";

		file.close();
		cout << "SELECT " << sc << " RESULTS:\n";
		file.close();
		for (int i = 0; i < this->nr_rows_indexed; i++) {
			if (p) table::tables_pointers[this->t_index]->print_row(sc, "Select/SELECT_" + to_string(sc) + ".txt", this->rows_indexed[i], p);
			else table::tables_pointers[this->t_index]->print_row(sc, "Select/SELECT_" + to_string(sc) + ".txt", this->rows_indexed[i]);
		}
		//}
	}

	static vector<table_index*> indexes;
	static int search_index(string& name) {

		if (table_index::indexes.empty()) return -1;
		for (int i = 0; i < table_index::indexes.size(); i++)
			if (table_index::indexes[i]->get_name() == name) return i;
		return -1;
	}
	static void filter_collums(string com, int i_table, int* p)
	{

		com.erase(0, 1); com.erase(com.size() - 1);
		int i = 0, j = 0; string sub;
		cout << "\n" << com;
		while (i < com.size() - 1)
		{
			j = com.find(',', i);
			if (j != -1) {
				sub.assign(com.begin() + i, com.begin() + j);
				cout << "\n" << sub;
			}
			else {
				sub.assign(com, i, com.size()); j = com.size() - 1;
				cout << "\n" << sub;
			}

			p[table::tables_pointers[i_table]->search_collumn(sub)]++;
			i = j + 1;
		}
		for (int l = 0; l < table::tables_pointers[i_table]->get_nr_coloane(); l++)
			cout << p[l] << " ";
	}
	static int cautare_index(string name)
	{
		if (table_index::indexes.empty()) return -1;
		for (int i = 0; i < table_index::indexes.size(); i++)
			if (table_index::indexes[i]->get_name() == name) return i;
		return -1;
	}
	void delete_row()
	{
		int* p = this->get_rows_indexed();



		for (int i = 0; i < this->get_nr_rows_indexed(); i++)
		{
			for (int j = 0; j < table::tables_pointers[this->t_index]->get_nr_coloane(); j++)
			{
				/*cout << "\n nr rows:  "<<table::tables_pointers[this->t_index]->get_coloane()[j]->get_nr_rows() << "\n";*/
				if (table::tables_pointers[this->t_index]->decide2(table::tables_pointers[this->t_index]->get_coloane()[j]))
					table::tables_pointers[this->t_index]->get_coloane()[j]->get_valori_char().erase(table::tables_pointers[this->t_index]->get_coloane()[j]->get_valori_char().begin() + p[i]);
				else table::tables_pointers[this->t_index]->get_coloane()[j]->get_valori_float().erase(table::tables_pointers[this->t_index]->get_coloane()[j]->get_valori_float().begin() + p[i]);
				table::tables_pointers[this->t_index]->get_coloane()[j]->get_nr_rows()--;

			}

			/*cout << "\n(1) Rows indexed: "<<this->get_nr_rows_indexed()<<  "\n";
			for (int l = 0; l < this->get_nr_rows_indexed(); l++)
			cout << this->get_rows_indexed()[l] << " ";*/

			if (i < this->get_nr_rows_indexed() - 1) {
				for (int k = i + 1; k < this->get_nr_rows_indexed(); k++)
					p[k]--;
			}

			/*cout << "\n(2) Rows indexed: " << this->get_nr_rows_indexed() << "\n";
			for (int l = 0; l < this->get_nr_rows_indexed(); l++)
				cout << this->get_rows_indexed()[l] << " ";*/

		}
	}
};
vector<table_index*> table_index::indexes;


