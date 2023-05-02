#include <iostream>
#include "BigInt.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <cstdlib>
#include <time.h>

using namespace std;

short patientID = 1;

struct Patient
{
    short ID;
    bool isIndividual; // is patient in group therapy
    bool firstTime = true;
    bool willHealFaster;            // will patient heal faster
    unsigned int timeInTherapy = 0; // how long patient is in therapy
    unsigned int neededTherapyTime; // how long patient needs to be in therapy
    unsigned int whenToReturn = 0;  // when patient will return
};

struct GroupT
{                               // group therapy
    bool isForFirstTime = true; // is group therapy for first time patients
    bool isFull;                // is group therapy full
    bool hasStarted = false;    // has group therapy started
    unsigned int startTime;     // when group therapy started
    unsigned int endTime;       // when group therapy ends
    vector<Patient> patients;   // patients in group therapy
};

struct IndividualT
{                            // individual therapy
    bool isFull;             // is individual therapy full
    bool hasStarted = false; // has individual therapy started
    unsigned int startTime;  // when individual therapy started
    unsigned int endTime;    // when individual therapy ends
    Patient patient;         // patient in individual therapy
};

struct State
{
    unsigned int n;         // specialists who form therapy groups out of k people
    unsigned int k;         // number of people in a group
    unsigned int nK;        // number of patients in a group (not first time)
    unsigned int m;         // specialists who work individually with patients
    unsigned int d;         // number of time it takes for a patient to come back
    unsigned int InT;       // InT - Individual therapy time in hours
    unsigned int GrT;       // GrT - Group therapy time in hours
    unsigned short p1;      // probability of a new patient arriving
    unsigned short p2;      // probability of a patient recovering faster
    unsigned short h;       // how much faster patients recover in percentage
    unsigned short p3;      // probability of a patient coming to individual therapy
    unsigned int GrN;       // GrN - number of group therapies required
    unsigned int InN;       // InN - number of individual therapies required
    BigInt InK;             // InK - price of individual therapy
    BigInt GrK;             // GrK - price of group therapy
    unsigned int sim_hours; // how many hours to simulate
};

struct Clinic
{
    vector<GroupT> groupTherapies;           // group therapies
    vector<IndividualT> individualTherapies; // individual therapies
    queue<Patient> patients;                 // patients in queue
    queue<Patient> indivWaiting;             // patients waiting for individual therapy
    queue<Patient> groupWaiting;             // patients waiting for group therapy
    queue<Patient> returningP;               // patients who will return
};

struct Revenue
{
    BigInt InR; // InR - revenue from individual therapy
    BigInt GrR; // GrR - revenue from group therapy
};

bool rand_prob(short prob)
{
    return ((rand() % 100) < prob) ? true : false;
}

void getClinicData(Clinic clinic)
{
    int size = clinic.patients.size();
    for (int i = 0; i < size; i++)
    {
        cout << "Patient " << i + 1 << " isIndividual: " << clinic.patients.front().isIndividual << " firstTime: " << clinic.patients.front().firstTime << endl;
        clinic.patients.pop();
    }
    size = clinic.individualTherapies.size();
    for (int i = 0; i < size; i++)
    {
        cout << "Individual therapy " << i + 1 << " isFull: " << clinic.individualTherapies[i].isFull << endl;
    }
    size = clinic.groupTherapies.size();
    for (int i = 0; i < size; i++)
    {
        cout << "Group therapy " << i + 1 << " isFull: " << clinic.groupTherapies[i].isFull << endl;
    }
}

int maxGroup(State state, Clinic clinic, bool isForNew) // returns group number with the biggest number of patients
{
    int max = 0;
    int maxGroup = 0;
    for (int i = 0; i < state.n; i++)
    {
        if (clinic.groupTherapies[i].isForFirstTime == isForNew && clinic.groupTherapies[i].patients.size() > max && !clinic.groupTherapies[i].isFull)
        {
            max = clinic.groupTherapies[i].patients.size();
            maxGroup = i;
        }
    }
    return maxGroup;
}

void writeStateData(State state)
{
    cout << "Olen Rackauskas. 3 grupe, 2 pogrupis." << endl;
    cout << "ADS 3 uzduotis. 10 variantas: Apsilankymas pas psichologa." << endl
         << endl;
    cout << "DALIS 1. Ivesti rodikliai." << endl;
    // cout << "Pradiniai duomenys: " << endl;
    cout << "Proceso trukme: " << state.sim_hours << " val." << endl;
    cout << "Privaciu psichologu skaicius: " << state.m << '.' << endl;
    cout << "Grupes psichologu skaicius: " << state.n << '.' << endl;
    cout << "Privacios terapijos trukme: " << state.InT << '.' << endl;
    cout << "Grupines terapijos trukme: " << state.GrT << '.' << endl;
    cout << "Zmoniu skaicius grupeje (nauji pacientai): " << state.k << '.' << endl;
    cout << "Zmoniu skaicius grupeje (pakartotinai ateinantys pacientai): " << state.nK << '.' << endl;
    cout << "Po kiek laiko grizta pacientai: " << state.d << '.' << endl;
    cout << "Sansas, kad ateis naujas pacientas: " << state.p1 << '.' << endl;
    cout << "Sansas, kad pacientas pasveiks greiciau: " << state.p2 << '.' << endl;
    cout << "Procentas, ant kiek greiciau pasveiks pacientas: " << state.h << "%." << endl;
    cout << "Sansas, kad pacientas pasirinks privacia terapija: " << state.p3 << '.' << endl;
    cout << "Kiek kartu reikia nueiti i grupines terapijas: " << state.GrN << '.' << endl;
    cout << "Kiek kartu reikia nueiti i individualias terapijas: " << state.InN << '.' << endl;
    cout << "Privaciu terapiju kaina: " << state.InK << " eur." << endl;
    cout << "Grupiniu terapiju kaina: " << state.GrK << " eur." << endl
         << endl;

    cout << "DALIS 2. Protokolas." << endl;
}

void getWaitAndReturnList(Clinic clinic)
{
    int gr = 0, in = 0;
    queue<Patient> temp(clinic.returningP);
    int size = temp.size();

    cout << "Laukianciu pacientu skaicius (grupines terapijos): " << clinic.groupWaiting.size() << endl;
    cout << "Laukianciu pacientu skaicius (privacios terapijos): " << clinic.indivWaiting.size() << endl;

    if (size > 0)
        for (int i = 0; i < size; i++)
        {
            if (temp.front().isIndividual)
                in++;
            else
                gr++;
            temp.pop();
        }

    cout << "Sugriztanciu pacientu skaicius (grupines terapijos): " << gr << endl;
    cout << "Sugriztanciu pacientu skaicius (privacios terapijos): " << in << endl;
}

Clinic checkIndividualPatients(State state, Clinic clinic)
{
    bool wasAdded = false;
    int size;
    size = clinic.indivWaiting.size(); // firstly checking the waiting list and adding patients through it
    if (size > 0)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; i < state.m; j++)
            {
                if (!clinic.individualTherapies[j].isFull)
                {
                    cout << "P" << clinic.indivWaiting.front().ID << " pridetas prie privacios terapijos. Specialisto ID: " << i+1 << endl;
                    clinic.individualTherapies[i].patient = clinic.indivWaiting.front();
                    clinic.individualTherapies[i].isFull = true;
                    clinic.indivWaiting.pop();
                    wasAdded = true;
                    break;
                }
            }
            if (!wasAdded)
            {
                cout << "Pacientai is laukimo eiles nebegali buti priimti." << endl;
                break;
            }
        }
    }
    wasAdded = false;
    size = clinic.patients.size(); // checking the main queue and adding patients through it
    if (size > 0)
    {
        // individual therapy
        if (clinic.patients.front().isIndividual == 1)
        {
            cout << "Atvyko P" << clinic.patients.front().ID << ". Pacientas nori privacios terapijos." << endl;
            // bool wasAdded = false;
            for (int i = 0; i < state.m; i++)
            {
                // cout << "Is full: " << clinic.individualTherapies[i].isFull << endl;
                if (!clinic.individualTherapies[i].isFull)
                {
                    clinic.individualTherapies[i].patient = clinic.patients.front();
                    clinic.individualTherapies[i].isFull = true;
                    clinic.patients.pop();
                    wasAdded = true;
                    cout << "Patient added" << endl;
                    break;
                }
            }
            if (!wasAdded)
            {
                cout << "P" << clinic.patients.front().ID << " laukia privacios terapijos." << endl;
                clinic.indivWaiting.push(clinic.patients.front());
                clinic.patients.pop();
            }
        }
    }
    return clinic;
}

void checkNewGroupPatients()
{
}

Revenue simulate(State state)
{
    writeStateData(state);
    srand(time(NULL));
    Revenue revenue = {
        .InR = BigInt("0"),
        .GrR = BigInt("0"),
    };
    Clinic clinic = {
        .groupTherapies = vector<GroupT>(state.n),
        .individualTherapies = vector<IndividualT>(state.m),
        .patients = queue<Patient>(),
        .indivWaiting = queue<Patient>(),
        .groupWaiting = queue<Patient>(),
        .returningP = queue<Patient>(),
    };
    int time = 0;
    int patients = 0;
    while (time < state.sim_hours)
    {
        cout << "T= " << time << '.' << endl;
        // new patient
        if (rand_prob(state.p1))
        {
            // cout << "New patient arrived" << endl;
            patients++;
            Patient patient = {
                .ID = patientID,
                .isIndividual = rand_prob(state.p3),
                .firstTime = true,
                .willHealFaster = rand_prob(state.p2),
                .timeInTherapy = 0,
            };
            patientID++;
            // cout << "Is indiv: " << endl;

            if (patient.isIndividual)
                patient.neededTherapyTime = state.InN;
            else
                patient.neededTherapyTime = state.GrN;

            if (patient.willHealFaster && !patient.isIndividual && state.GrN > 1)
            {
                patient.neededTherapyTime = state.GrN * (state.h / 100.0);
                // cout << "NEEDED THERAPY TIME (group): " << patient.neededTherapyTime << endl;
            }
            clinic.patients.push(patient);
            // cout << "Patient pushed." << endl;
        }

        clinic = checkIndividualPatients(state, clinic);

        int size = clinic.patients.size();
        // cout << "Size: " << size << endl;
        // Adds patients to their respective therapies
        if (size > 0)
        {
            // individual therapy
            // if (clinic.patients.front().isIndividual == 1)
            // {
            //     cout << "Atvyko P" << clinic.patients.front().ID << ". Pacientas nori privacios terapijos." << endl;
            //     bool wasAdded = false;
            //     if (clinic.indivWaiting.size() > 0)
            //     {
            //         // cout << "Patient waiting." << endl;
            //         for (int i = 0; i < state.m; i++)
            //         {
            //             if (!clinic.individualTherapies[i].isFull)
            //             {
            //                 clinic.individualTherapies[i].patient = clinic.indivWaiting.front();
            //                 clinic.individualTherapies[i].isFull = true;
            //                 clinic.indivWaiting.pop();
            //                 wasAdded = true;
            //                 break;
            //             }
            //         }
            //         if (!wasAdded)
            //         {
            //             // cout << "Patient was not added." << endl;
            //             clinic.indivWaiting.push(clinic.patients.front());
            //             clinic.patients.pop();
            //         }
            //     }
            //     else
            //     {
            //         for (int i = 0; i < state.m; i++)
            //         {
            //             if (!clinic.individualTherapies[i].isFull)
            //             {
            //                 clinic.individualTherapies[i].patient = clinic.patients.front();
            //                 clinic.individualTherapies[i].isFull = true;
            //                 clinic.patients.pop();
            //                 wasAdded = true;
            //                 break;
            //             }
            //         }
            //         if (!wasAdded)
            //         {
            //             clinic.indivWaiting.push(clinic.patients.front());
            //             clinic.patients.pop();
            //         }
            //     }
            // }
            // // group therapy
            // else
            // {
            if (clinic.patients.front().isIndividual == 0)
            {
                cout << "Atvyko P" << clinic.patients.front().ID << ". Pacientas nori grupines terapijos." << endl;
                bool wasAdded = false;
                int id;
                for (int i = maxGroup(state, clinic, true); i < state.n; i++)
                {
                    if (clinic.groupWaiting.size() > 0)
                    {

                        // cout << "Patient waiting." << endl;
                        if (clinic.groupTherapies[i].patients.size() < state.k && clinic.groupWaiting.front().firstTime == true && clinic.groupTherapies[i].isForFirstTime)
                        {
                            // cout << "Adding from waiting list" << endl;
                            clinic.groupTherapies[i].patients.push_back(clinic.groupWaiting.front());
                            clinic.groupTherapies[i].isForFirstTime = true;
                            cout << "P" << clinic.groupWaiting.front().ID << " yra priskirtas prie grupes " << i + 1 << endl;
                            clinic.groupWaiting.pop();
                            wasAdded = true;
                            cout << "Grupes " << i + 1 << " dydis = " << clinic.groupTherapies[i].patients.size() << '.' << endl;
                            if (clinic.groupTherapies[i].patients.size() == state.k)
                            {
                                clinic.groupTherapies[i].isFull = true;
                            }
                            break;
                        }
                        else if ((clinic.groupTherapies[i].patients.size() == 0 && clinic.groupWaiting.front().firstTime == false) || (clinic.groupTherapies[i].patients.size() > 0 && clinic.groupTherapies[i].isForFirstTime == false && clinic.groupWaiting.front().firstTime == false))
                        {
                            cout << "Adding from waiting list" << endl;
                            clinic.groupTherapies[i].patients.push_back(clinic.groupWaiting.front());
                            clinic.groupTherapies[i].isForFirstTime = false;
                            clinic.groupWaiting.pop();
                            wasAdded = true;
                            cout << "Assigned to group therapy " << i + 1 << endl;
                            cout << "Group size: " << clinic.groupTherapies[i].patients.size() << endl;
                            if (clinic.groupTherapies[i].patients.size() == state.k)
                            {
                                clinic.groupTherapies[i].isFull = true;
                            }
                            break;
                        }
                    }
                    else
                    {
                        if (clinic.groupTherapies[i].patients.size() < state.k && clinic.patients.front().firstTime == true && clinic.groupTherapies[i].isForFirstTime)
                        {
                            // cout << "Adding from patients list" << endl;
                            clinic.groupTherapies[i].patients.push_back(clinic.patients.front());
                            clinic.groupTherapies[i].isForFirstTime = true;
                            cout << "P" << clinic.patients.front().ID << " yra priskirtas prie grupes " << i + 1 << ". Jos dydis = " << clinic.groupTherapies[i].patients.size() << " pacientu." << endl;
                            clinic.patients.pop();
                            wasAdded = true;
                            // cout << "Assigned to group therapy " << i + 1 << endl;
                            // cout << "Grupes " << i+1 << " dydis = " << clinic.groupTherapies[i].patients.size() << endl;
                            if (clinic.groupTherapies[i].patients.size() == state.k)
                            {
                                clinic.groupTherapies[i].isFull = true;
                            }
                            break;
                        }
                    }
                    if (!wasAdded)
                        cout << "Patient (ID: " << clinic.patients.front().ID << ") was not added to group therapy" << endl;

                    // TODO In group therapy add a check for when patients are returning (make return only groups)
                }
                if (!wasAdded)
                {
                    cout << "Patient was not added" << endl;
                    clinic.groupWaiting.push(clinic.patients.front());
                    clinic.patients.pop();
                }
            }
        }
        size = clinic.returningP.size();
        bool wasAdded = false;

        if (size > 0)
        {
            // cout << "Size of returningP: " << size << endl;
            if (clinic.returningP.front().isIndividual)
            {
                if (clinic.returningP.front().whenToReturn == time)
                    cout << "Patient (ID: " << clinic.returningP.front().ID << ") returning to individual therapy" << endl;
                for (int i = 0; i < state.m; i++)
                {
                    if (!clinic.individualTherapies[i].isFull && clinic.returningP.front().whenToReturn == time)
                    {
                        clinic.individualTherapies[i].patient = clinic.returningP.front();
                        clinic.individualTherapies[i].isFull = true;
                        clinic.returningP.pop();
                        wasAdded = true;
                        cout << "Adding returning indiv patient to " << i + 1 << "th therapist" << endl;
                        break;
                    }
                }
                if (!wasAdded && clinic.returningP.front().whenToReturn == time)
                {
                    cout << "Returning indiv patient not added" << endl;
                }
            }
            if (clinic.returningP.size() > 0)
            {
                if (!clinic.returningP.front().isIndividual)
                {
                    wasAdded = false;
                    for (int j = maxGroup(state, clinic, false); j < state.n; j++)
                    {
                        if (clinic.returningP.size() == 0)
                            break;
                        for (int i = 0; i < size; i++)
                        {
                            // if (clinic.returningP.front().whenToReturn == time)
                            //     cout << "Patient (ID: " << clinic.returningP.front().ID << ") returning to group therapy" << endl;
                            if ((clinic.groupTherapies[j].patients.size() == 0 && clinic.returningP.front().whenToReturn == time) || (clinic.groupTherapies[j].patients.size() < state.nK && clinic.groupTherapies[j].isForFirstTime == false && clinic.returningP.front().whenToReturn == time))
                            {
                                cout << "Adding a returning patient (ID: " << clinic.returningP.front().ID << ") to group therapy " << j + 1 << endl;
                                clinic.groupTherapies[j].patients.push_back(clinic.returningP.front());
                                clinic.groupTherapies[j].isForFirstTime = false;
                                clinic.returningP.pop();
                                wasAdded = true;
                                cout << "Group size: " << clinic.groupTherapies[j].patients.size() << endl;
                                if (clinic.groupTherapies[j].patients.size() == state.nK)
                                {
                                    clinic.groupTherapies[j].isFull = true;
                                }
                            }
                        }
                    }
                    if (!wasAdded && clinic.returningP.front().whenToReturn == time)
                        cout << "Returning patient not added" << endl;
                }
            }
        }

        // TODO add returning patients to group therapies
        // TODO check if patients heal faster in group therapy
        // therapy simulations
        for (int i = 0; i < state.n; i++) // group therapies
        {
            if (clinic.groupTherapies[i].isFull && !clinic.groupTherapies[i].hasStarted)
            {
                cout << "Group therapy started. Group ID: " << i + 1 << endl;
                clinic.groupTherapies[i].hasStarted = true;
                clinic.groupTherapies[i].startTime = time;
                clinic.groupTherapies[i].endTime = time + state.GrT;
            }
            else if (clinic.groupTherapies[i].hasStarted && time == clinic.groupTherapies[i].endTime)
            {
                cout << "Group therapy ended. Group ID: " << i + 1 << endl;
                clinic.groupTherapies[i].hasStarted = false;
                clinic.groupTherapies[i].isFull = false;
                clinic.groupTherapies[i].isForFirstTime = true;
                size = clinic.groupTherapies[i].patients.size();
                for (int j = 0; j < size; j++)
                {
                    int fastHeal = state.GrN * (state.h / 100.0);
                    clinic.groupTherapies[i].patients[j].timeInTherapy++;
                    if (clinic.groupTherapies[i].patients[j].neededTherapyTime == state.GrN)
                    {
                        if (clinic.groupTherapies[i].patients[j].timeInTherapy < state.GrN)
                        {
                            clinic.groupTherapies[i].patients[j].firstTime = false;
                            clinic.groupTherapies[i].patients[j].whenToReturn = time + state.d;
                            clinic.returningP.push(clinic.groupTherapies[i].patients[j]);
                        }
                        else if (clinic.groupTherapies[i].patients[j].timeInTherapy == state.GrN)
                        {
                            cout << "Patient (ID: " << clinic.groupTherapies[i].patients[j].ID << ") is now fine" << endl;
                            revenue.GrR += state.GrK;
                        }
                    }
                    else
                    {
                        if (clinic.groupTherapies[i].patients[j].timeInTherapy < fastHeal)
                        {
                            clinic.groupTherapies[i].patients[j].firstTime = false;
                            clinic.groupTherapies[i].patients[j].whenToReturn = time + state.d;
                            clinic.returningP.push(clinic.groupTherapies[i].patients[j]);
                        }
                        else if (clinic.groupTherapies[i].patients[j].timeInTherapy == fastHeal)
                        {
                            cout << "Patient (ID: " << clinic.groupTherapies[i].patients[j].ID << ") is now fine" << endl;
                            revenue.GrR += state.GrK;
                        }
                    }
                }

                clinic.groupTherapies[i].patients.clear();
            }
        }

        for (int i = 0; i < state.m; i++) // individual therapies
        {
            if (clinic.individualTherapies[i].isFull && !clinic.individualTherapies[i].hasStarted)
            {
                cout << "Privati terapija su P" << clinic.individualTherapies[i].patient.ID << " prasidejo. Privataus specialisto ID: " << i + 1 << endl;
                clinic.individualTherapies[i].hasStarted = true;
                clinic.individualTherapies[i].startTime = time;
                clinic.individualTherapies[i].endTime = time + state.InT;
            }
            else if (clinic.individualTherapies[i].hasStarted && time == clinic.individualTherapies[i].endTime)
            {
                cout << "Privati terapija su P" << clinic.individualTherapies[i].patient.ID << " pasibaige. Privataus specialisto ID: " << i + 1 << endl;
                clinic.individualTherapies[i].hasStarted = false;
                clinic.individualTherapies[i].isFull = false;
                clinic.individualTherapies[i].patient.timeInTherapy++;
                if (clinic.individualTherapies[i].patient.timeInTherapy < state.InN)
                {
                    clinic.individualTherapies[i].patient.firstTime = false;
                    clinic.individualTherapies[i].patient.whenToReturn = time + state.d;
                    clinic.returningP.push(clinic.individualTherapies[i].patient);
                }
                else if (clinic.individualTherapies[i].patient.timeInTherapy == state.InN)
                {
                    cout << "Patient (ID: " << clinic.individualTherapies[i].patient.ID << ") is now fine" << endl;
                    revenue.InR += state.InK;
                }
            }
        }

        ++time;
        cout << endl;
    }
    getWaitAndReturnList(clinic);
    // getClinicData(clinic);
    return revenue;
}

int main()
{
    State s = {
        .n = 2,
        .k = 5,
        .nK = 3,
        .m = 1,
        .d = 2,
        .InT = 2,
        .GrT = 5,
        .p1 = 70,
        .p2 = 50,
        .h = 80,
        .p3 = 100,
        .GrN = 2,
        .InN = 2,
        .InK = BigInt("100000000"),
        .GrK = BigInt("200000000"),
        .sim_hours = 100,
    };
    Revenue r = simulate(s);
    cout << "Revenue: " << r.GrR << endl;
    cout << "Revenue: " << r.InR << endl;
    return 0;
}