#include "quest.h"
#include "mysql.h"
#include "main.h"
#include "io/file/csv.h"

#define JOB_VAGRANT 1
#define JOB_MERCENARY 2
#define JOB_ACROBAT 4
#define JOB_ASSIST 8
#define JOB_MAGICIAN 16
#define JOB_KNIGHT 32
#define JOB_BLADE 64
#define JOB_BILLPOSTER 128
#define JOB_RINGMASTER 256
#define JOB_PSYCHIKEEPER 512
#define JOB_ELEMENTOR 1024
#define JOB_JESTER 2048
#define JOB_RANGER 4096
#define JOB_PUPPETIER 8192
#define JOB_DOPPLER 16384
#define JOB_GATEKEEPER 32768

std::array<int, 16> tquest::joblookup = {JOB_VAGRANT, JOB_MERCENARY, JOB_ACROBAT, JOB_ASSIST, JOB_MAGICIAN, JOB_PUPPETIER, JOB_KNIGHT, JOB_BLADE, JOB_JESTER, JOB_RANGER, JOB_RINGMASTER, JOB_BILLPOSTER, JOB_PSYCHIKEEPER, JOB_ELEMENTOR, JOB_DOPPLER, JOB_GATEKEEPER};

tquest *q=0;
std::map<int, tquest*> tquest::quests;
pmutex tquest::questsmutex;

#define QIT_BEGIN 1
#define QIT_BEGINADD 2
#define QIT_END 3
#define QIT_REMOVE 4
#define QIT_REWARD 5

static void loadQuests(std::map<int, tquest*>&);
static void loadQuestDialogs(const std::map<int, tquest*>&);
static void loadQuestItems(const std::map<int, tquest*>&);
static void loadQuestDrops(const std::map<int, tquest*>&);

void tquest::initquests()
{
	logger.log("Quest data\n");
	loadQuests(quests);

	logger.log("Quest data - Dialogs\n");
	loadQuestDialogs(quests);

	logger.log("Quest data - Items\n");
    loadQuestItems(quests);

    logger.log("Quest data - Drops\n");
    loadQuestDrops(quests);
}

void loadQuests(std::map<int, tquest*> &quests)
{
    int a,b;
    sqlquery s1(connections[0].get(), "quests");
    s1.select("*, length(prevquests), length(killmobs), length(enditems), length(rewarditems), length(beginitems), length(toremoveitems), length(beginadditems), length(tquestitems)");

    int q_questid=s1.getColumnIndex("questid");
    int q_beginstate=s1.getColumnIndex("beginstate");
    int q_startlevel=s1.getColumnIndex("startlevel");
    int q_endlevel=s1.getColumnIndex("endlevel");
    int q_startcharacter=s1.getColumnIndex("startcharacter");
    int q_endcharacter=s1.getColumnIndex("endcharacter");
    int q_title=s1.getColumnIndex("title");
    int q_endstate=s1.getColumnIndex("endstate");
    int q_beginjob=s1.getColumnIndex("beginjob");
    int q_rewardjob=s1.getColumnIndex("rewardjob");
    int q_disquise=s1.getColumnIndex("disquise");
    int q_karmamin=s1.getColumnIndex("karmamin");
    int q_karmamax=s1.getColumnIndex("karmamax");
    int q_endkarmamin=s1.getColumnIndex("endkarmamin");
    int q_endkarmamax=s1.getColumnIndex("endkarmamax");
    int q_rewardmoneymin=s1.getColumnIndex("rewardmoneymin");
    int q_rewardmoneymax=s1.getColumnIndex("rewardmoneymax");
    int q_rewardexpmin=s1.getColumnIndex("rewardexpmin");
    int q_rewardexpmax=s1.getColumnIndex("rewardexpmax");
    int q_beginflags=s1.getColumnIndex("beginflags");
    int q_endflags=s1.getColumnIndex("endflags");
    int q_repeatable=s1.getColumnIndex("repeatable");

    while(s1.next())
    {
        a=toInt(s1[q_questid]);
        b=toInt(s1[q_beginstate]);
        q=new tquest(a);
        quests.insert(std::pair<int, tquest*>(a*256+b, q));
//			logger.log("%d %d %d\n", a, b, a*256+b);
        q->startlevel=toInt(s1[q_startlevel]);
        q->endlevel=toInt(s1[q_endlevel]);
        q->startcharacter=s1[q_startcharacter];
        q->endcharacter=s1[q_endcharacter];
        q->title=s1[q_title];
        q->beginstate=b;
        q->beginflags=toInt(s1[q_beginflags]);
        q->endflags=toInt(s1[q_endflags]);
        q->endstate=toInt(s1[q_endstate]);
        q->beginjob=toInt(s1[q_beginjob]);
        q->rewardjob=toInt(s1[q_rewardjob]);
        q->disquise=toInt(s1[q_disquise]);
        q->karmamin=toInt(s1[q_karmamin]);
        q->karmamax=toInt(s1[q_karmamax]);
        q->endkarmamin=toInt(s1[q_endkarmamin]);
        q->endkarmamax=toInt(s1[q_endkarmamax]);
        q->rewardmoneymin=toInt(s1[q_rewardmoneymin]);
        q->rewardmoneymax=toInt(s1[q_rewardmoneymax]);
        q->rewardexpmin=toInt(s1[q_rewardexpmin]);
        q->rewardexpmax=toInt(s1[q_rewardexpmax]);
        q->repeatable=(toInt(s1[q_repeatable])!=0);

        q->dialog_accept=toInt(s1["daccept"]);//5;
        q->dialog_deny=toInt(s1["ddeny"]);//6;
        q->dialog_finished=toInt(s1["dfinished"]);//7;
        q->dialog_askbefore=toInt(s1["daskbefore"]);//5
        a=toInt(s1["killmob1"]);
        if(a>0)
        {
            q->killmobs.push_back(std::pair<int, int>(a, toInt(s1["nkillmob1"])));
            a=toInt(s1["killmob2"]);
            if(a>0)
            {
                q->killmobs.push_back(std::pair<int, int>(a, toInt(s1["nkillmob2"])));
                a=toInt(s1["killmob3"]);
                if(a>0)
                {
                    q->killmobs.push_back(std::pair<int, int>(a, toInt(s1["nkillmob3"])));
                    a=toInt(s1["killmob4"]);
                    if(a>0)
                    {
                        q->killmobs.push_back(std::pair<int, int>(a, toInt(s1["nkillmob4"])));
                    }
                }
            }
        }

        char *i=s1.getptr("prevquests");
        b=toInt(s1["length(prevquests)"])/sizeof(int);
        if((b>0)&&(i!=0))
            for(a=0;a<b;a++)
                q->prevquests.push_back(*(int*)i);i+=sizeof(int);

    }
    s1.freeup();
}

void loadQuestDialogs(const std::map<int, tquest*> &quests)
{
    io::CSVReader<4, io::trim_chars<' '>, io::double_quote_escape<',','\"'> > reader("resources/questdialogs.csv");
    reader.read_header(io::ignore_extra_column, "questid", "dialognum", "str", "state");

    int questId, dialogNum, state;
    std::string dialog;

    while (reader.read_row(questId, dialogNum, dialog, state))
    {
        auto quest = std::find_if(quests.begin(), quests.end(), [questId, state] (std::pair<int, tquest*> pair) -> bool
        {
            return pair.first == (questId*256 + state);
        });

        if (quest != std::end(quests))
        {
            quest->second->dialogs.insert(std::pair<int, std::string>(dialogNum, dialog));
        }
        else
        {
            logger.log("No quest for dialog %d %d\n", questId, state);
        }
    }
}

void loadQuestItems(const std::map<int, tquest*> &quests)
{
    io::CSVReader<5> reader("resources/questitems.csv");
    reader.read_header(io::ignore_extra_column, "questid", "queststate", "itemid", "itemnumber", "actiontype");

    int questId, questState, itemId, itemNumber, actionType;

    while (reader.read_row(questId, questState, itemId, itemNumber, actionType))
    {
        auto quest = std::find_if(quests.begin(), quests.end(), [questId, questState] (std::pair<int, tquest*> pair) -> bool
        {
            return pair.first == (questId*256 + questState);
        });

        if (quest == std::end(quests))
        {
            logger.log("[questitems]: Quest id %d state %d not exists\n", questId, questState);
        }
        else
        {
            switch (actionType)
            {
                case QIT_BEGIN:
                    quest->second->beginitems.push_back(tquest::qitem(itemId, itemNumber));
                    break;
                case QIT_BEGINADD:
                    quest->second->beginadditems.push_back(tquest::qitem(itemId, itemNumber));
                    break;
                case QIT_END:
                    quest->second->enditems.push_back(tquest::qitem(itemId, itemNumber));
                    break;
                case QIT_REMOVE:
                    quest->second->toremoveitems.push_back(tquest::qitem(itemId, itemNumber));
                    break;
                case QIT_REWARD:
                    quest->second->rewarditems.push_back(tquest::qitem(itemId, itemNumber));
                    break;
                default:
                    logger.log("Unknown actiontype(%d) for quest item at (%d,%d)\n", actionType, questId, questState);
            }
        }
    }
}

void loadQuestDrops(const std::map<int, tquest*> &quests)
{
    io::CSVReader<6> reader("resources/questdrops.csv");
    reader.read_header(io::ignore_extra_column, "questid", "queststate", "monsterid", "itemid", "itemnumber", "dropchance");

    int questId, questState, monsterId, itemId, itemNumber;
    float dropChance;

    while (reader.read_row(questId, questState, monsterId, itemId, itemNumber, dropChance))
    {
        auto quest = std::find_if(quests.begin(), quests.end(), [questId, questState] (std::pair<int, tquest*> pair) -> bool
        {
            return pair.first == (questId*256 + questState);
        });

        if (quest == std::end(quests))
        {
            logger.log("[questdrops]: Quest id %d state %d not exists\n", questId, questState);
        }
        else
        {
            quest->second->questitems.insert(std::pair<int, tquest::tquestitems>(
                    monsterId,
                    tquest::tquestitems(monsterId, itemId, static_cast<int>(dropChance*100), itemNumber ) ));
        }
    }
}