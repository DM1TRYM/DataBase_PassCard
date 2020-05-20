#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef long long Int32;

typedef enum { Select, Del, Update, Uniq, Sort } OperationType;

int mallocCount = 0;
int reallocCount = 0;
int freeCount = 0;

typedef struct
{
    char* str;
} String;

String* CreateString()
{
    String* s = (char*)malloc(sizeof(char));
    s->str = (char*)malloc(sizeof(char));
    mallocCount += 2;
    s->str[0] = 0;
    return s;
}

void FreeString(String* string)
{
    string->str = NULL;
    free(string->str);
    freeCount++;
    free(string);
    freeCount++;
}

void AddToString(String* s, char c)
{
    int size = strlen(s->str);
    s->str = (char*)realloc(s->str, sizeof(char) * (size + 1));
    reallocCount++;
    s->str[size] = c;
    s->str[++size] = 0;
}

String* getString(FILE* F)
{
    String* str = CreateString();
    char c;
    if ((c = getc(F)) != '\"')
        return NULL;
    while (1)
    {
        c = getc(F);
        if ((c == '\"' && str->str[strlen(str->str) - 1] != '\\') || c == EOF || c == '\n')
            break;
        AddToString(str, c);
    }
    //if (c == '\"')
    //Добавить проверку на Ошибка закрытия строки кавычкой
    return str;
}

typedef struct
{
    char* phoneNumber;
} Phone;

Phone* CreatePhone()
{
    Phone* p = (Phone*)malloc(sizeof(Phone));
    mallocCount++;
    p->phoneNumber = (char*)malloc(sizeof(char));
    mallocCount++;
    p->phoneNumber[0] = 0;
    return p;
}

void FreePhone(Phone* phone)
{
    phone->phoneNumber = NULL;
    free(phone->phoneNumber);
    freeCount++;
    free(phone);
    freeCount++;
}

Phone* getPhone(FILE* F)
{
    Phone* p = CreatePhone();
    char buf[256];
    int i = 0;
    char c;
    if ((c = getc(F)) != '\'')
        return NULL;
    while (1)
    {
        c = getc(F);
        if (c == '\'' || c == EOF || c == '\n')
            break;
        buf[i++] = c;
    }
    buf[i] = 0;
    p->phoneNumber = (char*)realloc(p->phoneNumber, sizeof(char) * strlen(buf));
    reallocCount++;
    strcpy(p->phoneNumber, buf);
    //if (c == '\'')
    //Добавить проверку на Ошибка закрытия строки кавычкой
    return p;
}

int CheckPhone(Phone phone)
{
    int i = 0;
    int er = 0;
    int digit = 0;
    int bracket = 0;
    int size = strlen(phone.phoneNumber);
    
    if ((phone.phoneNumber[0] != '+' || phone.phoneNumber[1] != '7') && phone.phoneNumber[0] != '8')
        er++;
    
    for (i = 1; i < size; i++)
    {
        if (phone.phoneNumber[i] == '+')
            er++;
    }
    
    for (i = 0; i < 10; i++)
    {
        if (phone.phoneNumber[i] == '(')
        {
            er++;
            if (phone.phoneNumber[i + 4] == ')' || phone.phoneNumber[i + 6] == ')')
                er--;
        }
    }
    
    for (i = 0; i < size; i++)
    {
        if (isdigit(phone.phoneNumber[i]))
            digit++;
    }
    if (digit != 11)
        er++;
    
    for (i = 0; i < size; i++)
    {
        if (phone.phoneNumber[i] == '(')
            bracket++;
    }
    if (bracket > 1)
        er++;
    
    if (er == 0)
        return 1;
    else
        return 0;
}

char strchr0(char *s, int c)
{
    while (*s)
    {
        if (*s == c)
            return 1;
        s++;
    }
    return 0;
}

typedef struct
{
    char Number[9];
} Carnum;

void FreeCarnum(Carnum* carnum)
{
    free(carnum);
    freeCount++;
}

Carnum* getCarnum(FILE* F)
{
    Carnum* p = (Carnum*)malloc(sizeof(Carnum));
    p->Number[0] = '\0';
    int i = 0;
    char c;
    if ((c = getc(F)) != '\'')
        return NULL;
    while (1)
    {
        c = getc(F);
        if (c == '\'' || c == EOF || c == '\n')
            break;
        p->Number[i++] = c;
    }
    p->Number[i] = 0;
    //if (c == '\'')
    //Добавить проверку на Ошибка закрытия строки кавычкой
    return p;
}


int CheckCarnum(Carnum carnum)
{
    char mas[12] = "ABCEHKMOPTXY";
    int er = 0;
    if ((strchr0(mas, carnum.Number[0]) == 0) || (strchr0(mas, carnum.Number[4]) == 0) || (strchr0(mas, carnum.Number[5]) == 0))
        er++;
    if (!(isdigit(carnum.Number[1])) && !(isdigit(carnum.Number[2])) && !(isdigit(carnum.Number[3])))
        er++;
    if (!(isdigit(carnum.Number[6])) && !(isdigit(carnum.Number[7])))
        er++;
    if (!(isdigit(carnum.Number[8]) || carnum.Number[8] == '\0'))
        er++;
    if (carnum.Number[7] == '\0')
        er++;
    if (er == 0)
        return 1;
    else
        return 0;
}

//*****************************************************************************************

typedef struct
{
    char** items;
    int value;
} Enum;

Enum* CreateEnum()
{
    Enum* e = (Enum*)malloc(sizeof(Enum));
    e->value = -1;
    e->items = (char**)malloc(sizeof(char*) * 5);
    mallocCount++;
    e->items[0] = "active";
    e->items[1] = "disabled";
    e->items[2] = "vacation";
    e->items[3] = "lost";
    e->items[4] = "broken";
    return e;
}

void FreeEnum(Enum* _enum)
{
    int i;
    for (i = 0; i < 5; i++)
    {
        _enum->items[i] = NULL;
        free(_enum->items[i]);
        freeCount++;
    }
    free(_enum->items);
    freeCount++;
    free(_enum);
    freeCount++;
}

Enum* getEnum(FILE* F)
{
    Enum* e = CreateEnum();
    int i = 0;
    char c;
    char buf[128];
    if ((c = getc(F)) != '\'')
        return e;
    while (1)
    {
        c = getc(F);
        if (c == '\'' || c == EOF || c == '\n')
            break;
        buf[i++] = c;
    }
    buf[i] = 0;
    
    for (i = 0; i < 5; i++)
        if (strcmp(e->items[i], buf) == 0)
            e->value = i;
    
    //if (c == '\'')
    //Добавить проверку на Ошибка закрытия строки кавычкой
    return e;
}


typedef struct
{
    String* firstName;
    String* lastName;
    Phone* call;
    Carnum*  car;
    Enum* status;
    int id;
    int used;
} PassCard;

PassCard* CreatePassCard()
{
    PassCard* passCard = (PassCard*)malloc(sizeof(PassCard));
    mallocCount++;
    passCard->call = NULL;
    passCard->car = NULL;
    passCard->firstName = NULL;
    passCard->lastName = NULL;
    passCard->id = -1;
    passCard->used = -1;
    passCard->status = NULL;
    return passCard;
}

void FreePassCard(PassCard* passCard)
{
    if (passCard->firstName != NULL)
        FreeString(passCard->firstName);
    if (passCard->lastName != NULL)
        FreeString(passCard->lastName);
    if (passCard->call != NULL)
        FreePhone(passCard->call);
    if (passCard->car != NULL)
        FreeCarnum(passCard->car);
    if (passCard->status != NULL)
        FreeEnum(passCard->status);
    free(passCard);
    freeCount++;
}

//*****************************************************************************************
typedef struct
{
    PassCard* card;
    struct Element* next;
    struct Element* prev;
} Element;

typedef struct
{
    Element* data;
    Element* tail;
    int size;
} List;

void Add(List** list, PassCard* pc)
{
    Element* temp;
    if ((*list)->data == NULL)
    {
        (*list)->data = (Element*)malloc(sizeof(Element));
        mallocCount++;
        (*list)->data->card = pc;
        (*list)->data->next = NULL;
        (*list)->data->prev = NULL;
        (*list)->tail = (*list)->data;
        (*list)->size = 1;
        return;
    }
    temp = (Element*)malloc(sizeof(Element));
    mallocCount++;
    temp->next = NULL;
    temp->card = pc;
    temp->prev = (*list)->tail;
    
    (*list)->tail->next = temp;
    (*list)->tail = temp;
    (*list)->size++;
}

void Remove(List** list, Element* e)
{
    Element* prev = e->prev;
    Element* next = e->next;
    
    if (next == NULL && prev == NULL)
    {
        free(e);
        freeCount++;
        (*list)->data = NULL;
        (*list)->tail = NULL;
        (*list)->size = 0;
        return;
    }
    if (next == NULL)
    {
        prev->next = NULL;
        (*list)->tail = prev;
        (*list)->tail->next = NULL;
    }
    else
        if (prev == NULL)
        {
            next->prev = NULL;
            (*list)->data = next;
            (*list)->data->prev = NULL;
        }
        else
        {
            prev->next = next;
            next->prev = prev;
        }
}

List CreateDB()
{
    List list;
    list.data = NULL;
    list.size = 0;
    list.tail = list.data;
    return list;
}

void FreeDB(List** db)
{
    Element* t = *db;
    Element* pre;
    while (t != NULL)
    {
        pre = t->next;
        Remove(db, t);
        t = pre;
    }
}

//*****************************************************************************************
//Служебные функции

int getCommandIndex(char* command)
{
    if (strcmp(command, "insert") == 0)
        return 1;
    if (strcmp(command, "select") == 0)
        return 2;
    if (strcmp(command, "delete") == 0)
        return 3;
    if (strcmp(command, "update") == 0)
        return 4;
    if (strcmp(command, "uniq") == 0)
        return 5;
    if (strcmp(command, "sort") == 0)
        return 6;
    return 0;
}

char* getField(FILE* F, char c)
{
    char buf[256];
    char ch;
    int i = 0;
    while (isalpha(ch = getc(F)))
    {
        if (ch == EOF || ch == c || ch == '\n')
            break;
        buf[i++] = ch;
    }
    fseek(F, -1, SEEK_CUR);
    buf[i] = 0;
    return buf;
}

void PrintError(FILE *IFile, FILE *OFile)
{
    int i = 0;
    char c;
    fprintf(OFile, "incorrect:\'");
    while ((c = getc(IFile)) && i < 20)
    {
        if (c == '\n')
            break;
        fputc(c, OFile);
        i++;
    }
    if (c != '\n')
    {
        while ((c = getc(IFile)) != '\n')
            if (c == EOF)
                break;
    }
    fputc('\'', OFile);
    fputc('\n', OFile);
}

typedef struct
{
    char* fields[7];
    PassCard* values;
    PassCard* updateValues;
    char* cond[7];
    int condCount;
} Conditions;

void FreeCondition(Conditions* condition)
{
    int i;
    FreePassCard(condition->updateValues);
    for (i = 0; i < 7; i++)
    {
        condition->fields[i] = NULL;
        free(condition->fields[i]);
        freeCount++;
        condition->cond[i] = NULL;
        free(condition->cond[i]);
        freeCount++;
    }
    free(condition);
    freeCount++;
}

Conditions* CreateCondition()
{
    int i = 0;
    Conditions* c = (Conditions*)malloc(sizeof(Conditions));
    mallocCount++;
    c->condCount = 0;
    c->updateValues = CreatePassCard();
    c->values = CreatePassCard();
    for (i = 0; i < 7; i++)
    {
        c->fields[i] = 0;
        c->cond[i] = 0;
    }
    
    return c;
}


char* getCond(FILE* F)
{
    char buf[32];
    char c;
    int i = 0;
    while (c = getc(F))
    {
        if (c == EOF || c == '\n' || c == '\"' || c == '\'' || c == '[' || isdigit(c))
            break;
        buf[i++] = c;
    }
    fseek(F, -1, SEEK_CUR);
    buf[i] = 0;
    return buf;
}


int getFieldIndex(char* field)
{
    if (strcmp(field, "firstName") == 0)
        return 0;
    if (strcmp(field, "lastName") == 0)
        return 1;
    if (strcmp(field, "call") == 0)
        return 2;
    if (strcmp(field, "car") == 0)
        return 3;
    if (strcmp(field, "status") == 0)
        return 4;
    if (strcmp(field, "id") == 0)
        return 5;
    if (strcmp(field, "used") == 0)
        return 6;
    return -1;
}

Conditions* getCondition(FILE* F, OperationType type)
{
    Conditions* condition = CreateCondition();
    char tmp[128];
    int i = 0;
    int fieldIndex;
    char c;
    char field[256];
    char cond[32];
    
    if (type == Sort)
    {
        do
        {
            strcpy(tmp, getField(F, '='));
            fieldIndex = getFieldIndex(tmp);
            if (fieldIndex != -1 && fieldIndex != 4)
            {
                condition->fields[i] = (char*)malloc(sizeof(char) * strlen(tmp));
                strcpy(condition->fields[i], tmp);
                getc(F);
                strcpy(tmp, getField(F, ','));
                condition->cond[i] = (char*)malloc(sizeof(char) * strlen(tmp));
                strcpy(condition->cond[i], tmp);
                i++;
            }
            else
                return NULL;
            c = getc(F);
        } while (c == ',' && c != EOF);
    }
    
    if (type == Select || type == Uniq)
    {
        do
        {
            strcpy(tmp, getField(F, ','));
            condition->fields[i] = (char*)malloc(sizeof(char) * strlen(tmp));
            mallocCount++;
            strcpy(condition->fields[i], tmp);
            c = getc(F);
            i++;
        } while (c != ' ' && c != '\n' && c != EOF);
    }
    
    if (type == Update)
    {
        do
        {
            strcpy(field, getField(F, ' '));
            condition->fields[i] = (char*)malloc(sizeof(char) * strlen(field));
            mallocCount++;
            strcpy(condition->fields[i], field);
            fieldIndex = getFieldIndex(field);
            i++;
            if(getc(F) != '=')
                return NULL;
            switch (fieldIndex)
            {
                case 0:
                    condition->updateValues->firstName = getString(F);
                    if (condition->updateValues->firstName == NULL)
                        return NULL;
                    break;
                case 1:
                    condition->updateValues->lastName = getString(F);
                    if (condition->updateValues->lastName == NULL)
                        return NULL;
                    break;
                case 2:
                    condition->updateValues->call = getPhone(F);
                    if (condition->updateValues->call == NULL)
                        return NULL;
                    break;
                case 3:
                    condition->updateValues->car = getCarnum(F);
                    if (condition->updateValues->car == NULL)
                        return NULL;
                    break;
                case 4:
                    condition->updateValues->status = getEnum(F);
                    if (condition->updateValues->status == NULL)
                        return NULL;
                    break;
                case 5:
                    fscanf(F, "%i", &condition->updateValues->id);
                    if (condition->updateValues->id < 0)
                        return NULL;
                    break;
                case 6:
                    fscanf(F, "%i", &condition->updateValues->used);
                    if (condition->updateValues->firstName < 0 )
                        return NULL;
                    break;
                default:
                    break;
            }
            c = getc(F);
            if (c != ',' && c != ' ')
                return NULL;
        } while (c != ' ');
    }
    
    if (type == Select || type == Del || type == Update)
    {
        i = 0;
        while (c != -1 && c != '\n')
        {
            strcpy(field, getField(F, ' '));
            strcpy(cond, getCond(F));
            condition->condCount++;
            fieldIndex = getFieldIndex(field);
            condition->cond[fieldIndex] = (char*)malloc(sizeof(strlen(cond)));
            mallocCount++;
            strcpy(condition->cond[fieldIndex], cond);
            switch (fieldIndex)
            {
                case 0:
                    condition->values->firstName = getString(F);
                    if (condition->values->firstName == NULL)
                        return NULL;
                    break;
                case 1:
                    condition->values->lastName = getString(F);
                    if (condition->values->lastName == NULL)
                        return NULL;
                    break;
                case 2:
                    condition->values->call = getPhone(F);
                    if (condition->values->call == NULL)
                        return NULL;
                    break;
                case 3:
                    condition->values->car = getCarnum(F);
                    if (condition->values->car == NULL)
                        return NULL;
                    break;
                case 4:
                    condition->values->status = getEnum(F);
                    if (condition->values->status == NULL)
                        return NULL;
                    break;
                case 5:
                    if (fscanf(F, "%i", &condition->values->id) < 1)
                        return NULL;
                    break;
                case 6:
                    if (fscanf(F, "%i", &condition->values->used) < 1)
                        return NULL;
                    break;
                default:
                    break;
            }
            c = getc(F);
        }
    }
    return condition;
}

int StringCmp(char* cond, String s1, String s2)
{
    if (strcmp(cond, ">") == 0)
        return strcmp(s1.str, s2.str) > 0;
    if (strcmp(cond, "<") == 0)
        return strcmp(s1.str, s2.str) < 0;
    if (strcmp(cond, "==") == 0)
        return strcmp(s1.str, s2.str) == 0;
    if (strcmp(cond, "!=") == 0)
        return strcmp(s1.str, s2.str) != 0;
    if (strcmp(cond, ">=") == 0)
        return strcmp(s1.str, s2.str) > 0 || strcmp(s1.str, s2.str) == 0;
    if (strcmp(cond, "<=") == 0)
        return strcmp(s1.str, s2.str) < 0 || strcmp(s1.str, s2.str) == 0;
    
    return strcmp(s1.str, s2.str);
}

int IntCmp(char* cond, Int32 i1, Int32 i2)
{
    if (strcmp(cond, ">") == 0)
        return i1 > i2;
    if (strcmp(cond, "<") == 0)
        return i1 < i2;
    if (strcmp(cond, "==") == 0)
        return i1 == i2;
    if (strcmp(cond, "!=") == 0)
        return i1 != i2;
    if (strcmp(cond, ">=") == 0)
        return i1 > i2 || i1 == i2;
    if (strcmp(cond, "<=") == 0)
        return i1 < i2 || i1 == i2;
    if (i1 > i2)
        return 1;
    if (i1 < i2)
        return -1;
    return 0;
}

Int32 PhoneToNumber(Phone p1)
{
    Int32 result = 0;
    int i;
    for (i = 0; i < strlen(p1.phoneNumber); i++)
    {
        if (p1.phoneNumber[i] == '+')
        {
            i += 1;
            continue;
        }
        if (p1.phoneNumber[i] == '8' && i == 0)
            continue;
        if (isdigit(p1.phoneNumber[i]))
        {
            result = (result * 10) + ((int)p1.phoneNumber[i] - 48);
        }
    }
    return result;
}

int PhoneCmp(char* cond, Phone p1, Phone p2)
{
    Int32 pInt1 = PhoneToNumber(p1);
    Int32 pInt2 = PhoneToNumber(p2);
    return IntCmp(cond, pInt1, pInt2);
}

int CarnumCmp(char* cond, Carnum c1, Carnum c2)
{
    String *sNum1, *sNum2;
    int number1 = 0, number2 = 0, reg1 = 0, reg2 = 0;
    int i;
    sNum1 = CreateString();
    sNum2 = CreateString();
    for (i = 0; i < 6; i++)
    {
        if (isdigit(c1.Number[i]))
        {
            number1 = (number1 * 10) + ((int)c1.Number[i] - 48);
        }
        if (isalpha(c1.Number[i]))
        {
            AddToString(sNum1, c1.Number[i]);
        }
        if (isdigit(c2.Number[i]))
        {
            number2 = (number2 * 10) + ((int)c2.Number[i] - 48);
        }
        if (isalpha(c2.Number[i]))
        {
            AddToString(sNum2, c2.Number[i]);
        }
    }
    i = 6;
    while (c1.Number[i])
    {
        reg1 = (reg1 * 10) + ((int)c1.Number[i] - 48);
        i++;
    }
    i = 6;
    while (c2.Number[i])
    {
        reg2 = (reg2 * 10) + ((int)c2.Number[i] - 48);
        i++;
    }
    
    if (number1 == number2)
    {
        if (strcmp(sNum1, sNum2) == 0)
        {
            return IntCmp(cond, reg1, reg2);
        }
        else
        {
            return StringCmp(cond, *sNum1, *sNum2);
        }
    }
    else
    {
        return IntCmp(cond, number1, number2);
    }
}

int EnumCmp(char *cond, Enum e1, Enum e2)
{
    int i = 0;
    int isIn = 0;
    if (strcmp(cond, "==") == 0)
        return e1.value == e2.value;
    if (strcmp(cond, "!=") == 0)
        return e1.value != e2.value;
    while (i < 5)
    {
        if (strcmp(e2.items[i], e1.items[e1.value]) == 0)
            isIn = 1;
        i++;
    }
    
    if (strcmp(cond, "/in/") == 0)
        return isIn;
    if (strcmp(cond, "/not_in/") == 0)
        return !isIn;
}


int CheckCondition(Conditions condition, PassCard* card)
{
    int i;
    int error = 0;
    for (i = 0; i < 7; i++)
    {
        if (condition.cond[i])
        {
            switch (i)
            {
                case 0:
                    if (!StringCmp(condition.cond[i], *card->firstName, *condition.values->firstName))
                        error++;
                    break;
                case 1:
                    if (!StringCmp(condition.cond[i], *card->lastName, *condition.values->lastName))
                        error++;
                    break;
                case 2:
                    if (!PhoneCmp(condition.cond[i], *card->call, *condition.values->call))
                        error++;
                    break;
                case 3:
                    if (!CarnumCmp(condition.cond[i], *card->car, *condition.values->car))
                        error++;
                    break;
                case 4:
                    if (!EnumCmp(condition.cond[i], *card->status, *condition.values->status))
                        error++;
                    break;
                case 5:
                    if (!IntCmp(condition.cond[i], card->id, condition.values->id))
                        error++;
                    break;
                case 6:
                    if (!IntCmp(condition.cond[i], card->used, condition.values->used))
                        error++;
                    break;
            }
        }
        if (error > 0)
            return 0;
    }
    return 1;
}

void select(List** db, Conditions condition, FILE* F)
{
    Element* e = *db;
    int selected = 0;
    int i = 0;
    // @TODO Вывод кол-ва записей
    fputs("select:\n", F);
    
    while (e != NULL)
    {
        i = 0;
        if (condition.condCount == 0 || CheckCondition(condition, e->card))
        {
            while (condition.fields[i])
            {
                switch (getFieldIndex(condition.fields[i]))
                {
                    case 0:
                        fprintf(F,"%s=\"%s\" ", condition.fields[i], e->card->firstName->str);
                        break;
                    case 1:
                        fprintf(F, "%s=\"%s\" ", condition.fields[i], e->card->lastName->str);
                        break;
                    case 2:
                        fprintf(F, "%s=\'%s\' ", condition.fields[i], e->card->call->phoneNumber);
                        break;
                    case 3:
                        fprintf(F, "%s=\'%s\' ", condition.fields[i], e->card->car->Number);
                        break;
                    case 4:
                        fprintf(F, "%s=\'%s\' ", condition.fields[i], e->card->status->items[condition.values->status->value]);
                        break;
                    case 5:
                        fprintf(F, "%s=%i ", condition.fields[i], e->card->id);
                        break;
                    case 6:
                        fprintf(F, "%s=%i ", condition.fields[i], e->card->used);
                        break;
                    default:
                        break;
                }
                
                i++;
            }
            fputs("\n", F);
            selected++;
        }
        e = e->next;
    }
}

void Delete(List** db, Conditions condition, FILE* F)
{
    int deleted = 0;
    Element* e = (*db)->data;
    Element* parent;
    
    while (e != NULL)
    {
        if (CheckCondition(condition, e->card))
        {
            parent = e->next;
            Remove(db, e);
            deleted++;
            (*db)->size--;
            e = parent;
            continue;
        }
        e = e->next;
    }
    
    fprintf(F, "delete:%i\n", deleted);
}

void uniq(List** db, Conditions condition, FILE* F)
{
    int deleted = 0;
    Element* e = (*db)->tail;
    Element* q;
    Element* toDelete;
    int allRight = 0;
    int i = 0;
    
    while (e != NULL)
    {
        q = e->prev;
        while (q != NULL)
        {
            allRight = 1;
            i = 0;
            while (condition.fields[i])
            {
                switch (getFieldIndex(condition.fields[i]))
                {
                    case 0:
                        if (StringCmp("!=", *q->card->firstName, *e->card->firstName))
                            allRight = 0;
                        break;
                    case 1:
                        if (StringCmp("!=", *q->card->lastName, *e->card->lastName))
                            allRight = 0;
                        break;
                    case 2:
                        if (PhoneCmp("!=", *q->card->call, *e->card->call))
                            allRight = 0;
                        break;
                    case 3:
                        if (CarnumCmp("!=", *q->card->car, *e->card->car))
                            allRight = 0;
                        break;
                    case 4:
                        if (EnumCmp("!=", *q->card->status, *e->card->status))
                            allRight = 0;
                        break;
                    case 5:
                        if (IntCmp("!=", q->card->id, e->card->id))
                            allRight = 0;
                        break;
                    case 6:
                        if (IntCmp("!=", q->card->used, e->card->used))
                            allRight = 0;
                        break;
                    default:
                        break;
                }
                i++;
            }
            
            toDelete = q;
            q = q->prev;
            
            if (allRight)
            {
                Remove(db, toDelete);
                deleted++;
            }
        }
        e = e->prev;
    }
    
    fprintf(F, "uniq:%i\n", deleted);
}

void update(List** db, Conditions condition, FILE* F)
{
    int updated = 0;
    int i = 0;
    Element* e = *db;
    while (e != NULL)
    {
        if (CheckCondition(condition, e->card))
        {
            i = 0;
            while (condition.fields[i])
            {
                switch (getFieldIndex(condition.fields[i]))
                {
                    case 0:
                        e->card->firstName->str = (char*)realloc(e->card->firstName->str,
                                                                 sizeof(char) * strlen(condition.updateValues->firstName));
                        reallocCount++;
                        strcpy(e->card->firstName->str, condition.updateValues->firstName);
                        break;
                    case 1:
                        e->card->lastName->str = (char*)realloc(e->card->lastName->str,
                                                                sizeof(char) * strlen(condition.updateValues->lastName));
                        reallocCount++;
                        strcpy(e->card->lastName->str, condition.updateValues->lastName);
                        break;
                    case 2:
                        e->card->call->phoneNumber = (char*)realloc(e->card->call->phoneNumber,
                                                                    sizeof(char) * strlen(condition.updateValues->call->phoneNumber));
                        reallocCount++;
                        strcpy(e->card->call->phoneNumber, condition.updateValues->call->phoneNumber);
                        break;
                    case 3:
                        strcpy(e->card->car->Number, condition.updateValues->car->Number);
                        break;
                    case 4:
                        e->card->status->value = condition.updateValues->status->value;
                        break;
                    case 5:
                        e->card->id = condition.updateValues->id;
                        break;
                    case 6:
                        e->card->used = condition.updateValues->used;
                        break;
                    default:
                        break;
                }
                
                i++;
            }
            updated++;
        }
        e = e->next;
    }
    fprintf(F, "update:%i\n", updated);
}

void swap(Element *a, Element *b)
{
    if (a != b)
    {
        PassCard* tmp = a->card;
        a->card = b->card;
        b->card = tmp;
    }
}

void qsort(Element* left, Element* right, int d, int(*cmp)(const void*, const void*))
{
    if (left == right)
        return;
    else
        if (left->next == right)
        {
            if (cmp(left->card, right->card) > 0 && d)
                swap(left, right);
            if (cmp(left->card, right->card) < 0 && !d)
                swap(left, right);
        }
        else
        {
            Element* last = left;
            Element* current = left;
            do
            {
                current = current->next;
                if (cmp(current->card, left->card) < 0 && d)
                {
                    last = last->next;
                    swap(current, last);
                }
                if (cmp(current->card, left->card) > 0 && !d)
                {
                    last = last->next;
                    swap(current, last);
                }
            } while (current != right);
            swap(last, left);
            qsort(left, last, d, cmp);
            if (last != right)
                qsort(last->next, right, d, cmp);
        }
}

int FirstNameCmp(PassCard* p1, PassCard* p2)
{
    return StringCmp("", *p1->firstName, *p2->firstName);
}

int LastNameCmp(PassCard* p1, PassCard* p2)
{
    return StringCmp("", *p1->lastName, *p2->lastName);
}

int PhoneNumberCmp(PassCard* p1, PassCard* p2)
{
    return PhoneCmp("", *p1->call, *p2->call);
}

int CarNumberCmp(PassCard* p1, PassCard* p2)
{
    return CarnumCmp("", *p1->car, *p2->car);
}

int IDCmp(PassCard* p1, PassCard* p2)
{
    return IntCmp("", p1->id, p2->id);
}

int UsedCmp(PassCard* p1, PassCard* p2)
{
    return IntCmp("", p1->used, p2->used);
}

void QSort(List* list, int d, int(*cmp)(const void*, const void*))
{
    qsort(list->data, list->tail, d, cmp);
}

void sort(List** db, Conditions condition, FILE* F)
{
    int i = 0;
    int d;
    
    for (i = 0; i < 7; i++)
    {
        if (condition.cond[i])
        {
            d = strcmp(condition.cond[i], "asc") ? 0 : 1;
            switch (getFieldIndex(condition.fields[i]))
            {
                case 0:
                    QSort((*db), d, FirstNameCmp);
                    break;
                case 1:
                    QSort((*db), d, LastNameCmp);
                    break;
                case 2:
                    QSort((*db), d, PhoneNumberCmp);
                    break;
                case 3:
                    QSort((*db), d, CarNumberCmp);
                    break;
                case 5:
                    QSort((*db), d, IDCmp);
                    break;
                case 6:
                    QSort((*db), d, UsedCmp);
                    break;
                default:
                    break;
            }
        }
        i++;
    }
    
    fprintf(F, "sort:%i\n", (*db)->size);
}


void ReadFile(List* db)
{
    //*************************************
    FILE *InputFile, *OutputFile;
    PassCard* passCard;
    Conditions* condition;
    char command[256];
    char field[256];
    int i;
    //*************************************
    
    InputFile = fopen("input.txt", "r");
    OutputFile = fopen("output.txt", "w");
    
    if (!InputFile)
        return;
    
    while (!feof(InputFile))
    {
        fscanf(InputFile, "%s ", command);
        switch(getCommandIndex(command))
        {
            case 1:
                passCard = (PassCard*)malloc(sizeof(PassCard));
                for (i = 0; i < 7; i++)
                {
                    strcpy(field, getField(InputFile, '='));
                    if (fgetc(InputFile) != '=')
                        break;
                    if (strcmp(field, "firstName") == 0)
                    {
                        passCard->firstName = getString(InputFile);
                        if (passCard->firstName == NULL)
                            break;
                    }
                    if (strcmp(field, "lastName") == 0)
                    {
                        passCard->lastName = getString(InputFile);
                        if (passCard->lastName == NULL)
                            break;
                    }
                    if (strcmp(field, "call") == 0)
                    {
                        passCard->call = getPhone(InputFile);
                        if (passCard->call == NULL)
                            break;
                        if (CheckPhone(*passCard->call) == 0)
                            break;
                    }
                    if (strcmp(field, "car") == 0)
                    {
                        passCard->car = getCarnum(InputFile);
                        if (passCard->car == NULL)
                            break;
                        if (CheckCarnum(*passCard->car) == 0)
                            break;
                    }
                    if (strcmp(field, "status") == 0)
                    {
                        passCard->status = getEnum(InputFile);
                        if (passCard->status == NULL)
                            break;
                        if (passCard->status->value == -1)
                            break;
                    }
                    if (strcmp(field, "id") == 0)
                    {
                        if (fscanf(InputFile, "%i", &passCard->id) != 1)
                            break;
                    }
                    if (strcmp(field, "used") == 0)
                    {
                        if (fscanf(InputFile, "%i", &passCard->used) != 1)
                            break;
                    }
                    
                    if (fgetc(InputFile) != ',')
                        break;
                }
                if (i < 6)
                {
                    PrintError(InputFile, OutputFile);
                }
                else
                {
                    Add(&db, passCard);
                    fprintf(OutputFile, "insert:%i\n", db->size);
                }
                break;
            case 2:
                condition = getCondition(InputFile, Select);
                if (condition == NULL)
                {
                    PrintError(InputFile, OutputFile);
                    FreeCondition(condition);
                    continue;
                }
                select(db, *condition, OutputFile);
                FreeCondition(condition);
                break;
            case 3:
                condition = getCondition(InputFile, Del);
                if (condition == NULL)
                {
                    PrintError(InputFile, OutputFile);
                    FreeCondition(condition);
                    continue;
                }
                Delete(&db, *condition, OutputFile);
                FreeCondition(condition);
                break;
            case 4:
                condition = getCondition(InputFile, Update);
                if (condition == NULL)
                {
                    PrintError(InputFile, OutputFile);
                    FreeCondition(condition);
                    continue;
                }
                update(db, *condition, OutputFile);
                FreeCondition(condition);
                break;
            case 5:
                condition = getCondition(InputFile, Uniq);
                if (condition == NULL)
                {
                    PrintError(InputFile, OutputFile);
                    FreeCondition(condition);
                    continue;
                }
                uniq(&db, *condition, OutputFile);
                FreeCondition(condition);
                break;
            case 6:
                condition = getCondition(InputFile, Sort);
                if (condition == NULL)
                {
                    PrintError(InputFile, OutputFile);
                    FreeCondition(condition);
                    continue;
                }
                sort(&db, *condition, OutputFile);
                FreeCondition(condition);
                break;
        }
    }
    fclose(InputFile);
    fclose(OutputFile);
}


//*****************************************************************************************

void main()
{
    FILE* StatFile;
    StatFile = fopen("memstat.txt", "w");
    List dataBase = CreateDB();
    ReadFile(&dataBase);
    FreeDB(&dataBase);
    fprintf(StatFile, "malloc:%i\nrealloc:%i\nfree:%i", mallocCount, reallocCount, freeCount);
    fclose(StatFile);
}
