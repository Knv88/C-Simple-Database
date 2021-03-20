#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include "clase.h"
using namespace std;


class create_error {
	string error;
public:
	create_error()
	{
		error = "";
	}
	create_error(string error)
	{
		this->error = error;
	}
	string get_error()
	{
		return this->error;
	}
};
void read(string& com, fstream& file)
{
	if (file.is_open()) file >> com;
	else cin >> com;
}
void read_line(string& com, fstream& file)
{
	if (file.is_open()) getline(file, com);
	else getline(cin, com);
}
void create_table(string& com, fstream& file)
{
	try {

		read(com, file);
		if (!com.find('(', 0) || !com.find(',', 0)) { read_line(com, file);  throw create_error("\nTable name missing.\n"); }
		if (table::cautare_obiect(com) != -1) { read_line(com, file);  throw create_error("\nTable name already exists.\n"); }
		string name = com;
		read_line(com, file);
		int i = com.find('(', 0);
		com.assign(com, i, com.size() - i);
		com.erase(0, 1);
		com.erase(com.size() - 1, 1);
		int verify = 1;
		table cp(name, com, verify);
		if (!verify) throw create_error("\nIncomplete collumn description or wrong types.\n");
		table* p = new table;
		*p = cp;
		table::tables_pointers.push_back(p);
		cout << "\nTable created!\n";
	}
	catch (create_error er)
	{
		cout << er.get_error();
		//if (file.is_open())

			//getline(file, com);
		//else
			//getline(cin, com);
	}
}
void create_index(string& com, fstream& file)
{
	try {
		string index_name;
		read(index_name, file);
		//cout << index_name << " in index \n";
		if (table_index::search_index(index_name) != -1) throw create_error("\nIndex name already in use.\n");
		string table_name;
		read(table_name, file);
		//cout << table_name << " in index \n";
		int i_table = table::cautare_obiect(table_name);
		if (i_table == -1) throw create_error("\nThe table doesn`t exist.\n");

		string collum_name;
		read(collum_name, file);
		//cout << collum_name << " in index \n";
		int i_collum = table::tables_pointers[i_table]->search_collumn(collum_name);
		if (i_collum == -1) throw create_error("\nThe collum doesn`t exist\n");

		read(com, file);
		//cout << com << " in index \n";
		read(com, file);
		int start = 0;
		if (table::tables_pointers[i_table]->get_coloane()[i_collum]->search_row(&start, com) == -1) throw create_error("No such value was found.");
		table_index* p = new table_index;
		p->set_name(index_name);
		p->set_table_name(table_name);
		p->set_collum_name(i_table, i_collum, collum_name, &start, com);
		table_index::indexes.push_back(p);
		//p->print_index();
		cout << "\nIndex created!\n";
	}
	catch (create_error er)
	{
		cout << er.get_error();
		if (file.is_open())

			getline(file, com);
		else
			getline(cin, com);
	}

}
void drop_table(string& com, fstream& file)
{
	try {
		read_line(com, file);
		com.erase(0, 1);
		int i_table = table::cautare_obiect(com);
		if (i_table == -1) throw create_error("\nTable not found.\n");
		delete table::tables_pointers[i_table];
		table::tables_pointers.erase(table::tables_pointers.begin() + i_table);
		cout << "\nTable deleted!\n";

		for (int i = 0; i < table_index::indexes.size(); i++)
			if (table_index::indexes[i]->get_name() == com)
			{
				delete table_index::indexes[i];
				table_index::indexes.erase(table_index::indexes.begin() + i);
			}

		remove(("Tables/" + com + ".txt").c_str());
		remove(("Tables/" + com + "_data.bin").c_str());
	}
	catch (create_error er)
	{
		cout << er.get_error();
	}
}
void drop_index(string& com, fstream& file)
{
	try {
		read(com, file);
		int i_index = table_index::cautare_index(com);
		if (i_index == -1) throw create_error("\nIndex not found.\n");
		delete table_index::indexes[i_index];
		table_index::indexes.erase(table_index::indexes.begin() + i_index);
		cout << "\nIndex deleted!\n";
	}
	catch (create_error er)
	{
		cout << er.get_error();
		getline(cin, com);
		fflush;
	}
}

void create_comanda_2(string& com, fstream& file, string v = "")
{
	try {
		read(com, file);
		cout << com << " in create \n";

		if (com == "TABLE") create_table(com, file);
		else if (com == "INDEX") create_index(com, file);
		else  create_error("\nWrong keywords.\n");
	}
	catch (create_error er)
	{
		cout << er.get_error();
		if (v.empty())

			getline(cin, com);
		else
			getline(file, com);
	}
}
void drop_comanda_2(string& com, fstream& file, string v = "")
{
	try {
		read(com, file);

		if (com == "TABLE") drop_table(com, file);
		else if (com == "INDEX") drop_index(com, file);
		else  create_error("\nWrong keywords.\n");
	}
	catch (create_error er)
	{
		cout << er.get_error();
		if (v.empty())

			getline(cin, com);
		else
			getline(file, com);
	}
}
void display(int& dc, string& com, fstream& file, string v = "")
{
	try {
		read(com, file);
		if (com != "TABLE") throw create_error("\nWrong keyword.\n");
		read_line(com, file);
		com.erase(0, 1);
		cout << com;
		int index = table::cautare_obiect(com);

		if (index == -1)   throw create_error("\nTable not found\n");
		else
			table::tables_pointers[index]->print(dc); dc++;


	}
	catch (create_error er)
	{
		cout << er.get_error();
	}
}
bool numara_virgule(string sub, int i_table)
{
	int virgule = 0;
	for (int i = 0; i < sub.size(); i++)
		if (sub.at(i) == ',') virgule++;
	if (virgule == table::tables_pointers[i_table]->get_nr_coloane()) return 1;
	else return 0;
}
void insert(string& com, fstream& file, string v = "")
{
	try {
		read(com, file);						//into
		//cout << com << " in insert 1\n";

		if (com != "INTO") {
			read_line(com, file); throw create_error("\nMissing INTO.\n");
		}


		read(com, file);                        //users
		//cout << com << " in insert 2\n";

		if (com == "VALUES") { read_line(com, file); throw create_error("\nMissing table.\n"); }
		int i_table = table::cautare_obiect(com);
		if (i_table == -1) {
			read_line(com, file);  throw create_error("\nTable not found.\n");
		}
		read(com, file);						//values
		//cout << com << " in insert 3\n";
		if (com != "VALUES") {
			read_line(com, file); throw create_error("\nVALUES Missing.\n");
		}

		read_line(com, file);
		//cout << com << " 4 \n";

		com.erase(0, 1);
		unsigned int firstpoz = 0;
		string sub;
		while (firstpoz < com.size() - 1)
		{
			int lastpoz = com.find(')', firstpoz);
			if (lastpoz == -1) throw create_error("\nMissing last ')'\n");
			sub.assign(com.begin() + firstpoz, com.begin() + lastpoz + 1);

			if (sub.at(0) != '(') throw create_error("\nMissing first'('\n");
			if (numara_virgule(sub, i_table)) throw create_error("\nMissing ','.\n");
			table::tables_pointers[i_table]->adaugare(sub, i_table);
			firstpoz = com.find('(', lastpoz + 1);
			if (firstpoz == -1) firstpoz = com.size() - 1;
		}
		cout << "\nRows added!.\n";

	}
	catch (create_error er)
	{
		cout << er.get_error();

	}
}

void select(string& com, fstream& file, int& sc, string v = "")
{
	try {
		read_line(com, file);
		com.erase(0, 1);
		//cout << com << "in select\n";
		string sub; int i = 0, j = 0;
		j = com.find(' ', i + 1); sub.assign(com.begin() + i, com.begin() + j);

		int is = 0; string collumn_names;
		if (sub == "ALL") {
			is = 1;
			if (com.find('(') != -1)
				throw create_error("\nALL not allowed with collums.\n");
		}


		if (sub.size() <= 2) throw create_error("\nMissing collumn.\n");
		if (!is) collumn_names = sub;
		//cout << "\n" << collumn_names << "\n";
		i = j + 1; j = com.find(' ', i); sub.assign(com.begin() + i, com.begin() + j);

		if (sub != "FROM") throw create_error("\nMissing FROM\n");

		i = j + 1; j = com.find(' ', i);
		if (j != -1)sub.assign(com.begin() + i, com.begin() + j);
		else {
			sub.assign(com, i, com.size() - i); j = com.size() - 1;
		}

		int i_table = table::cautare_obiect(sub);
		if (i_table == -1) throw create_error("\nWrong table name.\n");

		if (j < com.size() - 1) {
			i = j + 1; j = com.find(' ', i); sub.assign(com.begin() + i, com.begin() + j);
			if (sub != "WHERE") throw create_error("\nMissing WHERE.\n");							//with where



			i = j + 1; j = com.find(' ', i); sub.assign(com.begin() + i, com.begin() + j);
			string filter_collum = sub;


			int i_collum = table::tables_pointers[i_table]->search_collumn(filter_collum);
			if (com.find(',', i) != -1) throw create_error("\nmultiple WHERE criteria\n");


			i = j + 3;  sub.assign(com, i, com.size() - i);



			if (sub.at(0) == '"') { sub.erase(0, 1); sub.erase(sub.size() - 1, 1); }
			table_index index1(i_table, i_collum, sub);
			if (is) { index1.print_index(sc); sc++; }   //all with where
			else {
				int* filter = new int[table::tables_pointers[i_table]->get_nr_coloane()]{ 0 };       //costum collums
				table_index::filter_collums(collumn_names, i_table, filter);
				for (int i = 0; i < table::tables_pointers[i_table]->get_nr_coloane(); i++)
					if (i) break;
				if (i == table::tables_pointers[i_table]->get_nr_coloane() - 1) throw create_error("\nCollumns not found\n");
				index1.print_index(sc, filter);
				sc++;
				delete[] filter;
			}

		}
		else {
			if (is) {
				table::tables_pointers[i_table]->print(sc, 1); sc++;		// all 						//no where
			}
			else {
				int* filter = new int[table::tables_pointers[i_table]->get_nr_coloane()]{ 0 };
				table_index::filter_collums(collumn_names, i_table, filter);
				for (int i = 0; i < table::tables_pointers[i_table]->get_nr_coloane(); i++)
					if (i) break;
				if (i == table::tables_pointers[i_table]->get_nr_coloane() - 1) throw create_error("\nCollumns not found\n");//costum collums no where
				table::tables_pointers[i_table]->print(sc, 1, filter);
				sc++;
				delete[] filter;
			}
		}


	}
	catch (create_error er)
	{
		cout << er.get_error();



	}
}
void update(string& com, fstream& file, string v = "") {
	try {
		read(com, file);
		//cout << com << " in update 1\n";

		int i_table = table::cautare_obiect(com);
		if (i_table == -1) throw create_error("\nTable doesn`t exist\n");

		read(com, file);
		//cout << com << " in update 2\n";

		if (com != "SET") throw create_error("\nMissing SET.\n");

		read(com, file);
		//cout << com << " in update 3\n";

		if (com == "WHERE") throw create_error("\nMissing collumn to change.\n");
		int i_c1 = table::tables_pointers[i_table]->search_collumn(com);
		if (i_c1 == -1) throw create_error("\nCollum not found.\n");
		read(com, file);
		//cout << com << " in update 4\n";

		read(com, file);
		//cout << com << " in update 5\n";

		if (com.at(0) == '"') {
			com.erase(0, 1);
			com.erase(com.size() - 1, 1);
		}
		//cout << com << " in update 5\n";
		string v1 = com;

		read(com, file);
		//cout << com << " in update 6\n";

		if (com == ",") throw create_error("\nToo many SET columns.\n");
		if (com != "WHERE") throw create_error("\nWHERE missing.\n");

		read(com, file);
		//cout << com << " in update 7\n";
		int i_c2 = 0;
		if (com.at(com.size() - 1) == '=')
		{
			com.erase(com.size() - 1, 1);
			i_c2 = table::tables_pointers[i_table]->search_collumn(com);
			if (i_c2 == -1) throw create_error("\nCollum not found.\n");
		}
		else {
			//cout << "\nno =\n";
			i_c2 = table::tables_pointers[i_table]->search_collumn(com);
			if (i_c2 == -1) throw create_error("\nCollum not found.\n");
			read(com, file);
		}
		//cout << i_c2 << "\n";
		//cout << com << " in update 8\n";

		read(com, file);
		//cout << com << " in update 9\n";


		if (com.at(0) == '"') {
			com.erase(0, 1);
			com.erase(com.size() - 1, 1);
		}
		//cout << com << " in update 9\n";
		string v2 = com;

		read_line(com, file);
		//cout << com << " in update 10\n";
		if (com.find(',') != -1) throw create_error("\nToo many arguments for WHERE.\n");
		//cout << i_c2 << "\n";
		table::tables_pointers[i_table]->update_row(i_c1, v1, i_c2, v2);
		cout << "\nValues updated!.\n";

	}
	catch (create_error er)
	{
		cout << er.get_error();
		if (file.is_open())

			getline(file, com);
		else
			getline(cin, com);
	}
}
void import_data(string& com, fstream& file)
{
	try {
		read(com, file);
		int index = table::cautare_obiect(com);
		if (index == -1) throw create_error("\nTable not found");
		read(com, file);
		ifstream csv(com);
		if (!csv) throw create_error("\nCSV file not open.\n");
		string line;
		while (csv >> line)
		{
			table::tables_pointers[index]->adaugare(line, index, 1);
		}
	}
	catch (create_error er)
	{
		cout << er.get_error();
		if (file.is_open())

			getline(file, com);
		else
			getline(cin, com);
	}
}
void delete_collumn(string& com, fstream& file, string v = "")
{
	try {
		read(com, file);
		//cout << com << " in delete 1\n";
		if (com != "FROM") throw create_error("\nFROM missing or wrong keyword.\n");
		read(com, file);
		//cout << com << " in delete 2\n";
		if (com == "WHERE") throw create_error("\nMissing table name.\n");
		int i_table = table::cautare_obiect(com);
		if (i_table == -1) throw create_error("\nWrong table name.\n");
		read(com, file);
		//cout << com << " in delete 3\n";
		if (com != "WHERE") throw create_error("\nWrong table or missing WHERE.\n");
		read(com, file);
		//cout << com << " in delete 4\n";
		int i_col = table::tables_pointers[i_table]->search_collumn(com);
		if (i_col == -1) throw create_error("\nCollumn not found.\n");
		//read(com, file);

		read(com, file);
		//cout << com << " in delete 5\n";
		read(com, file);
		//cout << com << " in delete 6\n";
		if (com.at(0) == '"') { com.erase(0, 1); com.erase(com.size() - 1, 1); }
		string value = com;
		read_line(com, file);
		if (com.find(',') != -1) throw create_error("\nToo many WHERE conditions\n");
		table_index del(i_table, i_col, value);
		del.delete_row();
		cout << "\nCollumn deleted!\n";
	}
	catch (create_error er)
	{
		cout << er.get_error();
		if (file.is_open())

			getline(file, com);
		else
			getline(cin, com);
	}
}

report** resize_report_pointers(report** report_pointers,int& nr_report)
{
	report** copy = new report * [nr_report+1];
	for (int i = 0; i < nr_report; i++)
		copy[i] = report_pointers[i];
	report_pointers = copy;
	return report_pointers;
}
void menu_1(report** report_pointers, int &nr_reports)
{
	resize_report_pointers(report_pointers, nr_reports);
	db_report* r = new db_report("Reports/", nr_reports + 1);
	report_pointers[nr_reports] = r;
	report_pointers[nr_reports]->generate_raport();
	nr_reports++;
	cout << "\nReport generated.\n";
}
void menu_2(int& dc, int& sc)
{
	int j = 1, i = 1;
	while (j <= dc && i <= sc)
	{
		remove(("Display/DISPLAY_" + to_string((int)j) + ".txt").c_str()); j++;
		remove(("Select/SELECT_" + to_string((int)i) + ".txt").c_str()); i++;

	}
	while (j <= dc) { remove(("Display/DISPLAY_" + to_string((int)j) + ".txt").c_str()); j++; }
	while (i <= sc) { remove(("Select/SELECT_" + to_string((int)i) + ".txt").c_str()); i++; }


	dc = sc = 1;

	ofstream tables("Tables/saved_tables.txt");
	tables << sc << " " << dc << endl;
	for (int i = 0; i < table::tables_pointers.size(); i++) {
		
		tables << table::tables_pointers[i]->getname() << endl;
	}
	tables.close();
	cout << "\nFiles deleted.\n";
}
void menu_3(int&dc,int&sc,int& reset)
{
	menu_2(dc, sc);
	for (int i = 0; i < table::tables_pointers.size(); i++)
	{
		remove(("Tables/"+ table::tables_pointers[i]->getname() + ".txt").c_str());
		remove(("Tables/" + table::tables_pointers[i]->getname() + "_data.bin").c_str());
	}
	reset = 1;
	remove("Tables/saved_tables.txt");
	cout << "\nThe database has been reset.\n";
}


int menu(string& com,fstream& file,report** report_pointers,int& nr_reports,int& dc,int& sc,int& reset)
{
	cout << "\nThere are 3 functions:\n   1->Generate raport of the database.\n   2->Delete Display and Select files.\n   3->Delete the database.\nEnter numeric key or type exit to return to basic commands.\n";
	while (1)
	{
		read(com, file);
		if (com == "exit") break;
		else if (com == "1") menu_1(report_pointers,nr_reports);
		else if (com == "2") menu_2(dc,sc);
		else if (com == "3") menu_3(dc,sc,reset);
	}
	return 0;
}

void Citire_Comanda_1(string com, fstream& file, int& sc, int& dc,report** report_pointers,int& nr_reports, int& reset,string v = "")
{
	try {
		if (com == "CREATE") create_comanda_2(com, file, v);
		else if (com == "DROP") drop_comanda_2(com, file, v);
		else if (com == "DISPLAY") display(dc, com, file, v);
		else if (com == "INSERT") insert(com, file, v);
		else if (com == "SELECT") select(com, file, sc, v);
		else if (com == "UPDATE") update(com, file, v);
		else if (com == "DELETE") delete_collumn(com, file, v);
		else if (com == "IMPORT") import_data(com, file);
		else if (com == "MENU") menu(com,file,report_pointers,nr_reports,sc,dc,reset);
		else
			throw create_error("\nWrong keyword.\n");
	}
	catch (create_error er)
	{
		cout << er.get_error();
		if (v.empty())

			getline(cin, com);
		else
			getline(file, com);
	}

}
void delete_reports(report** report_pointers, int nr_reports)
{
	for (int i = 0; i < nr_reports; i++)
	{
		remove(("Reports/" + report_pointers[i]->get_name()).c_str());
		delete report_pointers[i];
	}
}
bool save(int& sc, int& dc)
{
	ofstream tables("Tables/saved_tables.txt");
	tables << sc << " " << dc << endl;
	for (int i = 0; i < table::tables_pointers.size(); i++) {
		files saving(table::tables_pointers[i]->getname());
		tables << table::tables_pointers[i]->getname() << endl;
	}
	tables.close();
	return 0;

}
void load_saves(int& sc, int& dc) {
	ifstream saved_tables("Tables/saved_tables.txt");
	if (saved_tables) cout << "saved tables open\n";

	saved_tables >> sc >> dc;

	string table_name;
	while (saved_tables >> table_name)
	{
		cout << table_name << "\n";
		ifstream conf_data("Tables/" + table_name + ".txt");
		if (conf_data) cout << "conf open\n";
		else cout << "Configuration data not open.\n";
		string name, nr_col, nr_row;
		string col_name, type, def, dim;
		conf_data >> name; cout << name << "\n";
		conf_data >> nr_col >> nr_row; cout << nr_col << " " << nr_row << "\n";

		table cp(name, stoi(nr_col), stoi(nr_row));
		table* p = new table;
		*p = cp;
		table::tables_pointers.push_back(p);
		int index = table::cautare_obiect(table_name);
		//cout << "table created from saves\n";

		ifstream bin_data("Tables/" + table_name + "_data.bin", ios::binary | ios::in);
		if (bin_data) cout << "Bin file open.\n";
		else cout << "Bin file not open.\n";

		for (int i = 0; i < stoi(nr_col); i++)
		{
			conf_data >> col_name >> type >> def >> dim;
			//cout << col_name << " " << type << " " << def << " " << dim << "\n";
			collumn cc(col_name, type, def, dim, nr_row);
			collumn* col = new collumn;
			*col = cc;
			table::tables_pointers[index]->put_collum(col);
			//cout << "col created\n";




			for (int j = 0; j < stoi(nr_row); j++)
			{
				int t_dim;
				string value;
				conf_data >> t_dim;
				//cout << t_dim << " dim\n";
				getline(bin_data, value, '\0');
				//cout << value << " value\n";
				table::tables_pointers[index]->get_coloane()[i]->decide(value, 1);
				/*	if (table::tables_pointers[index]->decide2(table::tables_pointers[index]->get_coloane()[i]))
						table::tables_pointers[index]->get_coloane()[i]->push_value(value);
					else table::tables_pointers[index]->get_coloane()[i]->push_value_float(value);*/

			}

			cout << endl;
		}
		bin_data.close();
		conf_data.close();

		/*cout << table::tables_pointers[index]->getname() << " " << table::tables_pointers[index]->get_nr_coloane() << " \n";
		for (int i = 0; i < table::tables_pointers[index]->get_nr_coloane(); i++)
		{
			cout << "name is: " << table::tables_pointers[index]->get_coloane()[i]->getname() << "\n";
		}

		cout << table::cautare_obiect(table_name) << " \n";
		cout << "the size is:" << table::tables_pointers[index]->get_coloane().size() << "\n";*/
	}
	cout << "\nSaves loaded!\n";
}
int main(int argc, char** argv)
{
	//cout << "before save\n";
	int select_count = 1, display_count = 1;
	load_saves(select_count, display_count);
	//cout << "after save\n";
	int nr_reports = 0;
	int reset = 0;
	report** reports_pointers = nullptr;
	reports_pointers = new report * [1];
	string com;

	if (argc > 1)
	{
		int i = 1;
		string filename;

		while (i < argc)
		{
			filename = argv[i];
			fstream file(filename);
			if (file.is_open()) cout << "File is open.\n";
			else cout << "The file cannot be opened\n";
			while (file >> com)
			{
				//cout << com << " in main \n";
				Citire_Comanda_1(com, file, select_count, display_count,reports_pointers,nr_reports,reset, "not null");

			}
			file.close();
			i++;
		}
		//return 0;
	}
	//else {
	fstream test;
	do {
		cout << "Input command or type exit to save and close: \n";;
		cin >> com;
		if (com == "exit") break;
		Citire_Comanda_1(com, test, select_count, display_count,reports_pointers, nr_reports,reset);
	} while (com != "exit");
	if (nr_reports) delete_reports(reports_pointers, nr_reports);
	if (reset) return 0;
	 else return save(select_count, display_count);
	//}
}
//The project is incomplete,we are sorry but and we didn`t have time to code operators or change vectors into arrays 
//The project is designed a week ago and it`s something like sql lite for linux with tables having rows and collumns
//the insert command appends more rows ,doesn`t delete the existing values and it can receive multiple rows at once
// examples we debugged with:
//CREATE TABLE USERS ((FIRST_NAME,text,10,'John') , (LAST_NAME,text,10,'Doe') , (pass,text,10,'0000') , (id,integer,10,'0'))

//INSERT INTO USERS VALUES ("Ella","Steward","al567",1), ("Ela","Ryan","tr643",2) , ("Ela","Molina","78653t",3) , ("Ella","Harrison","78653t",4) , ("Ella","Oliver","bt6y",5)
//CREATE INDEX FIRST USERS id = 5
//SELECT (FIRST_NAME,pass) FROM USERS WHERE id = 5
//UPDATE USERS SET id = 9 WHERE FIRST_NAME = "Ella"
//DELETE FROM USERS WHERE id = 5
//DISPLAY TABLE USERS
//DROP INDEX FIRST
//DROP TABLE USERS
///UPDATE USERS SET FIRST_NAME = "Ela" WHERE id = 1
//cd Desktop\source\Project_Phase3\Debug
