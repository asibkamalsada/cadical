#include <algorithm>
//
// Created by asib1 on 12.03.2021.
//

#include "../../src/cadical.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

int main(__unused int argc, char *argv[]) {
    auto *solver = new CaDiCaL::Solver;
    solver->set("quiet", 1);

    unordered_map<std::string, int> arg2lit;
    unordered_map<int, std::string> lit2arg;

#define B_SIZE 20

    char b[B_SIZE];

    char ch;
    fstream fin(argv[1], fstream::in);

    int argcount = 0;

    bool args_finished = false;

    int *pre_counts;

    unordered_map<int, int *> pre;

    while (fin >> noskipws >> ch) {
        if (ch == 'a') {
            fin >> noskipws >> ch;
            if (ch == 'r') {
                fin >> noskipws >> ch;
                if (ch == 'g') {
                    fin >> noskipws >> ch;
                    if (ch == '(') {
                        fin >> noskipws >> ch;
                        unsigned int counter = 0;
                        while (ch != ')') {
                            if (counter > B_SIZE) exit(1);
                            b[counter] = ch;
                            fin >> noskipws >> ch;
                            counter++;
                        }
                        argcount++;
                        std::string s(b, counter);
                        arg2lit[s] = argcount;
                        lit2arg[argcount] = s;
                    }
                }
            } else if (ch == 't') {
                fin >> noskipws >> ch;
                if (ch == 't') {
                    fin >> noskipws >> ch;
                    if (ch == '(') {
                        if (!args_finished) {
                            args_finished = true;
                            solver->reserve(argcount);
                            pre_counts = (int *) calloc(argcount, sizeof(int));
                            for (int i = 0; i < argcount; i++) {
                                pre[i] = new int[argcount];
                            }
                        }

                        int lit1;
                        int lit2;
                        fin >> noskipws >> ch;
                        unsigned int counter = 0;
                        while (ch != ',') {
                            if (counter > B_SIZE) exit(1);
                            b[counter] = ch;
                            fin >> noskipws >> ch;
                            counter++;
                        }
                        lit1 = arg2lit[std::string(b, counter)];
                        solver->add(-lit1);
                        fin >> noskipws >> ch;
                        counter = 0;
                        while (ch != ')') {
                            if (counter > B_SIZE) exit(1);
                            b[counter] = ch;
                            fin >> noskipws >> ch;
                            counter++;
                        }
                        lit2 = arg2lit[std::string(b, counter)];
                        solver->add(-lit2);
                        solver->add(0);

                        pre[lit2 - 1][pre_counts[lit2 - 1]] = lit1;
                        pre_counts[lit2 - 1]++;

                    }
                }
            }
        }
    }

    // there gotta be another way without caching pre (directly putting edges as positive literals maybe)
    for (int i = 0; i < argcount; i++) {
        for (int pre_count = 0; pre_count < pre_counts[i]; pre_count++) {
            solver->add(pre[i][pre_count]);
        }
        solver->add(i + 1);
        solver->add(0);
    }

    int sol_buff[argcount];

    while (solver->solve() == 10) {
        bool first_out = true;
        for (int lit = 1; lit < argcount + 1; lit++) {
            if ((sol_buff[lit - 1] = solver->val(lit)) > 0) {
                if (first_out) {
                    cout << '"' << lit2arg[lit];
                    first_out = false;
                } else {
                    cout << ' ' << lit2arg[lit];
                }
            }
        }
        if (first_out) cout << '"';
        cout << "\"\n";
        for (int signed_lit : sol_buff) {
            solver->add(-signed_lit);
        }
        solver->add(0);
    }

    delete solver;

    for (const auto &a : pre) {
        delete a.second;
    }

    delete pre_counts;

    return 0;
}
