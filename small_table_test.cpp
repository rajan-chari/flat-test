#include <iostream>
#include "generated/SmallTable_generated.h"
#include "generated/SmallTableCollection_generated.h"
using namespace FlatTest;
using namespace std;
using namespace flatbuffers;

DetachedBuffer get_serialized_buffer_stc(size_t count)
{
    FlatBufferBuilder fbb;
    vector<Offset<SmallTable>> vect_smallclass;
    for(int i = 0; i < count; i++){
        vect_smallclass.emplace_back(CreateSmallTable(fbb,i));
    }
    auto vec_offset = fbb.CreateVector(vect_smallclass);
    SmallTableCollectionBuilder sccb(fbb);
    sccb.add_c1(vec_offset);
    auto scc = sccb.Finish();
    fbb.Finish(scc);
    return fbb.Release();
}

void print_stats(const DetachedBuffer& db, int coll_sz)
{
    cout  << " collection size: " << coll_sz  << " buffer size: " << db.size() << ", sz/elm: " << (float)db.size()/coll_sz << endl;
}

DetachedBuffer get_serialized_buffer_st()
{
    FlatBufferBuilder fbb;
    SmallTableBuilder stb(fbb);
    stb.add_belment(255);
    auto st = stb.Finish();
    fbb.Finish(st);
    return fbb.Release();
}

void print_stats(const DetachedBuffer& db)
{
    cout  << " buffer size: " << db.size() << endl;
}

void small_table_test()
{
    cout  << "small_table_test" << endl;
    DetachedBuffer db = get_serialized_buffer_st();
    auto stc_read = GetSmallTable(db.data());
    auto readval = stc_read->belment();
    print_stats(db);
}

void small_table_collection_test()
{
    cout  << "small_table_collection_test" << endl;
    int coll_szs[] = {1,10,100,1000,10000,100000,1000000};
    for(auto coll_sz : coll_szs){
        DetachedBuffer db = get_serialized_buffer_stc(coll_sz);
        auto stc_read = GetSmallTableCollection(db.data());
        print_stats(db, coll_sz);
    }
}