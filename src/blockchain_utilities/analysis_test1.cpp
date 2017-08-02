// Andrew Miller, UIUC

#include "bootstrap_file.h"
#include "blocksdat_file.h"
#include "common/command_line.h"
#include "cryptonote_core/tx_pool.h"
#include "blockchain_db/blockchain_db.h"
#include "blockchain_db/lmdb/db_lmdb.h"
#if defined(BERKELEY_DB)
#include "blockchain_db/berkeleydb/db_bdb.h"
#endif
#include "blockchain_db/db_types.h"
#include "version.h"
#include <stdio.h>
#include <sqlite3.h>
#include <sstream>

namespace po = boost::program_options;
using namespace epee; // log_space

namespace {
std::string refresh_string = "\r                                    \r";
}

sqlite3* dbs;
char* zErrMsg = 0;
int rc;
char* sql;

bool find_output_in_tx(const cryptonote::transaction& tx, const crypto::public_key& output_pubkey, int& size){
    size_t idx {0};
    auto it = std::find_if(tx.vout.begin(), tx.vout.end(), [&](const cryptonote::tx_out& o){
        const txout_to_key& tx_in_to_key = boost::get<txout_to_key>(o.target);
        ++idx;
        return tx_in_to_key.key == output_pubkey;
    });
    if (it != tx.vout.end()){
        // we found the desired public key
        //out = *it;
        //output_index = idx > 0 ? idx - 1 : idx;
        //cout << idx << " " << output_index << endl;
        size = tx.vout.size();
        return true;
    }
    return false;
}

//CREATE TABLE out_table (tx_hash STRING, block_height INTEGER, amount INTEGER, g_idx INTEGER, timestamp INTEGER);";

bool setupTable()
{
    string sql_second = "PRAGMA synchronous = OFF; \
         PRAGMA journal_mode = MEMORY; \  
         CREATE TABLE out_table (block_height INTEGER, tx_hash STRING, tx_timestamp INTEGER, outkey STRING, idx INTEGER);";
         //CREATE TABLE in_table (block_height INTEGER, tx_timestamp INTEGER, tx_hash STRING, input_num INTEGER, mixin_num INTEGER, mixin_height INTEGER, mixin_timestamp INTEGER, mixin_pubkey STRING, mixin_tx STRING);";
         //CREATE TABLE first (tx_hash STRING, block_height INTEGER, amount INTEGER, mixin_height INTEGER, tx_timestamp INTEGER, mixin_timestamp INTEGER, mixin_tx STRING);";
    //         CREATE UNIQUE INDEX id ON second (tx_hash, input_no, mixin_no); \
    //         CREATE INDEX timestamp ON second (tx_hash_timestamp);";
    sql = const_cast<char*>(sql_second.c_str());

    /* Execute SQL statement */
    rc = sqlite3_exec(dbs, sql, NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stderr, "Creation done successfully\n");
    }
}

bool parse_blockchain_pass1(Blockchain* blockchain_storage, tx_memory_pool* _tx_pool, boost::filesystem::path& output_file, uint64_t requested_block_stop)
{
    uint64_t cur_height = 0;
    uint64_t num_blocks_written = 0;
    uint64_t progress_interval = 100;
    block b;

    uint64_t block_start = 1;
    uint64_t block_stop = 0;
/*    LOG_PRINT_L0("source blockchain height: " << blockchain_storage->get_current_blockchain_height() - 1);
*/    if ((requested_block_stop > 0) && (requested_block_stop < blockchain_storage->get_current_blockchain_height())) {
/*        LOG_PRINT_L0("Using requested block height: " << requested_block_stop);
*/        block_stop = requested_block_stop;
    }
    else {
        block_stop = blockchain_storage->get_current_blockchain_height() - 1;
/*        LOG_PRINT_L0("Using block height of source blockchain: " << block_stop);
*/    }
/*    LOG_PRINT_L0("Storing blocks raw data...");
*/
    // Map to store the current number of mixins associated with each output
    std::map<uint64_t, uint64_t> availableMixinsMap;

    //starts at 0, goes to top height of the block
    // CHANGE ME to the blocks you want to start and end.
    //for (cur_height = 110; cur_height <= 111; ++cur_height){
    /*1324000*/
    std::cout << "height,tx,extra," << "\n";
    for (cur_height = 1322564; cur_height <= block_stop; ++cur_height) {
        // this method's height refers to 0-based height (genesis block = height 0)
        crypto::hash hash = blockchain_storage->get_block_id_by_height(cur_height);

        cryptonote::block blk;

        sqlite3_exec(dbs, "BEGIN TRANSACTION", NULL, NULL, &zErrMsg);

        try {
            blk = blockchain_storage->get_db().get_block_from_height(cur_height);
        }
        catch (std::exception& e) {
            cerr << e.what() << endl;
            return false;
        }

        // get all transactions in the block found
        // initialize the first list with transaction for solving
        // the block i.e. coinbase.
        list<cryptonote::transaction> txs{ blk.miner_tx };
        list<crypto::hash> missed_txs;

        if (!blockchain_storage->get_transactions(blk.tx_hashes, txs, missed_txs)) {
            cerr << "Cant find transactions in block: " << cur_height << endl;
            return false;
        }

/*        LOG_PRINT_L0("block height: " << cur_height << " " << blk.timestamp);
*/
        for (const cryptonote::transaction& tx : txs) {
            crypto::hash tx_hash = get_transaction_hash(tx);

            // convert the tx_hash to string, so we can put into database
            stringstream tt;
            tt << tx_hash;
            std::string ttt = tt.str();
            ttt.erase(ttt.end() - 1);
            ttt.erase(ttt.begin());

            LOG_PRINT_L1("\ntx hash: " << tx_hash << ", block height " << cur_height);

            vector<string> mixin_timescales_str;

            // total number of inputs in the transaction tx
            size_t input_no = tx.vin.size();
            size_t output_no = tx.vout.size();
            std::vector<uint8_t>extra = tx.extra;


/*            std::cout << "Extra Field (as text): ";
            for(std::vector<uint8_t>::iterator xtra = extra.begin(); xtra != extra.end(); ++xtra) {
                std::cout << *xtra;
            }*/
            stringstream ex;
            for(std::vector<uint8_t>::iterator xtra = extra.begin(); xtra != extra.end(); ++xtra) {
                ex << std::hex << static_cast<int>(*xtra);
            }
            std::string ext = ex.str();
            std::string t("2210"); /*It seems that all payment IDs start with 2210*/
            if (ext.compare(0, t.length(), t) == 0){
                std::cout << cur_height << "," << ttt << ",";
                std::cout << ext << ",\n";
            }

            for (size_t in_i = 0; in_i < input_no; ++in_i) {
                cryptonote::txin_v tx_in = tx.vin[in_i];

                if (tx_in.type() == typeid(cryptonote::txin_gen)) {
                    LOG_PRINT_L1(" - coinbase tx: no inputs here.\n");
                    continue;
                }

                // get tx input key
                const cryptonote::txin_to_key& tx_in_to_key
                    = boost::get<cryptonote::txin_to_key>(tx_in);

                LOG_PRINT_L1("Input's xmr:" << tx_in_to_key.amount);

                size_t count = 0;

                uint64_t avail = availableMixinsMap[tx_in_to_key.amount];
                LOG_PRINT_L1("Available mixins:" << avail);

                std::vector<uint64_t> absolute_offsets = relative_output_offsets_to_absolute(tx_in_to_key.key_offsets);
                std::vector<cryptonote::output_data_t> outputs;
                blockchain_storage->get_db().get_output_key(tx_in_to_key.amount, absolute_offsets, outputs);

                // lookup the block offset
                for (const uint64_t& i : absolute_offsets) {

                    cryptonote::output_data_t output_data;
                    size_t output_index;
                    bool output_at{ true };

                    // get tx hash and output index for output
                    if (count < outputs.size()) {
                        output_data = outputs.at(count);
                    }

                    uint64_t mixin_height = output_data.height;
                    //LOG_PRINT_L0("size: " << outputs.size());
                    //LOG_PRINT_L0("public key: " << output_data.pubkey);

                    //here, we will attempt to find the tx_hash that the block came from
                    cryptonote::block mixin_blk;
                    mixin_blk = blockchain_storage->get_db().get_block_from_height(mixin_height);
                    //LOG_PRINT_L0("mixin_timestamp: " << mixin_blk.timestamp);

                    list<cryptonote::transaction> mxs{ mixin_blk.miner_tx };
                    list<crypto::hash> missed_mxs;
                    if (!blockchain_storage->get_transactions(mixin_blk.tx_hashes, mxs, missed_mxs)) {
                        cerr << "Cant find transactions in block: " << mixin_height << endl;
                        return false;
                    }

                    int tx_size;
                    crypto::hash m_hash;
                    for (const cryptonote::transaction& mx : mxs){
                        if (find_output_in_tx(mx, output_data.pubkey, tx_size)){
                            m_hash = get_transaction_hash(mx);
                            break;
                        }
                    }
                    //LOG_PRINT_L0("tx_size: " << tx_size);

                    //LOG_PRINT_L0("input_tx: " << m_hash);

                    stringstream ss;
                    ss << output_data.pubkey;
                    std::string sss = ss.str();
                    sss.erase(sss.end() - 1);
                    sss.erase(sss.begin());

                    stringstream xx;
                    xx << m_hash;
                    std::string xxx = xx.str();
                    xxx.erase(xxx.end() - 1);
                    xxx.erase(xxx.begin());

                    LOG_PRINT_L1("mixin no: " << (count + 1));
                    LOG_PRINT_L1("offset:" << i << " (" << float(i) / avail << ")");
/*                    string sql_second = "insert into in_table (block_height, tx_timestamp, tx_hash, input_num, mixin_num, mixin_height, mixin_timestamp, mixin_pubkey, mixin_tx) values(\""
                            + to_string(cur_height) + "\", \""
                            + to_string(blk.timestamp) + "\", \""
                            + ttt + "\", \""
                            + to_string(in_i+1) + "\", \""
                            + to_string(count+1) + "\", \""
                            + to_string(mixin_height) + "\", \""
	                        + to_string(mixin_blk.timestamp) + "\", \""
                            + sss + "\", \""
	                        + xxx + "\")";

	                    sql = const_cast<char*>(sql_second.c_str());

	                    rc = sqlite3_exec(dbs, sql, NULL, NULL, &zErrMsg);
	                    if (rc != SQLITE_OK) {
	                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
	                        sqlite3_free(zErrMsg);
	                    }*/
                    ++count;
                }
            }

            // Update the availableMixins table
            int idx = 0;
            for (size_t out_i = 0; out_i < tx.vout.size(); ++out_i) {
                cryptonote::tx_out tx_out = tx.vout[out_i];
                txout_target_v target = tx_out.target;
                txout_to_key txout_key = boost::get<txout_to_key>(target);
                crypto::public_key outkey = txout_key.key;
                LOG_PRINT_L1("Output key: " << outkey);
                availableMixinsMap[tx_out.amount]++;

                stringstream yy;
                yy << outkey;
                std::string yyy = yy.str();
                yyy.erase(yyy.end() - 1);
                yyy.erase(yyy.begin());

                string sql_second = "insert into out_table (block_height, tx_hash, tx_timestamp, outkey, idx) values(\""
                        + to_string(cur_height) + "\", \""
                        + ttt + "\", \""
                        + to_string(blk.timestamp) + "\", \""
                        + yyy + "\", \""                        
                        + to_string(idx) + "\")";

                sql = const_cast<char*>(sql_second.c_str());

                rc = sqlite3_exec(dbs, sql, NULL, NULL, &zErrMsg);
                    if (rc != SQLITE_OK) {
                        fprintf(stderr, "SQL error: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                        }
 				idx+=1;
            }

           sqlite3_exec(dbs, "COMMIT TRANSACTION", NULL, NULL, &zErrMsg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            else {
/*                fprintf(stdout, "Transaction committed successfully\n");
*/            }
        }

        //write_block(hash);
        if (cur_height % NUM_BLOCKS_PER_CHUNK == 0) {
            num_blocks_written += NUM_BLOCKS_PER_CHUNK;
        }

        if (cur_height % progress_interval == 0) {
/*            std::cout << refresh_string;
*/            //std::cout << "block " << cur_height << "/" << block_stop << std::flush;
        }
    }
    // print message for last block, which may not have been printed yet due to progress_interval
/*    std::cout << refresh_string;
    std::cout << "block " << cur_height - 1 << "/" << block_stop << ENDL;*/

    return true;
}

std::string join_set_strings(const std::unordered_set<std::string>& db_types_all, const char* delim)
{
    std::string result;
    std::ostringstream s;
    std::copy(db_types_all.begin(), db_types_all.end(), std::ostream_iterator<std::string>(s, delim));
    result = s.str();
    if (result.length() > 0)
        result.erase(result.end() - strlen(delim), result.end());
    return result;
}

int main(int argc, char* argv[])
{
    std::string default_db_type = "lmdb";

    std::unordered_set<std::string> db_types_all = cryptonote::blockchain_db_types;
    db_types_all.insert("memory");

    std::string available_dbs = join_set_strings(db_types_all, ", ");
    available_dbs = "available: " + available_dbs;

    uint32_t log_level = 0;
    uint64_t block_stop = 0;
    bool blocks_dat = false;

    tools::sanitize_locale();

    boost::filesystem::path default_data_path{ tools::get_default_data_dir() };
    boost::filesystem::path default_testnet_data_path{ default_data_path / "testnet" };
    boost::filesystem::path output_file_path;

    po::options_description desc_cmd_only("Command line options");
    po::options_description desc_cmd_sett("Command line options and settings options");
    const command_line::arg_descriptor<std::string> arg_output_file = { "output-file", "Specify output file", "", true };
    const command_line::arg_descriptor<uint32_t> arg_log_level = { "log-level", "", log_level };
    const command_line::arg_descriptor<uint64_t> arg_block_stop = { "block-stop", "Stop at block number", block_stop };
    const command_line::arg_descriptor<bool> arg_testnet_on = {
        "testnet", "Run on testnet.", false
    };
    const command_line::arg_descriptor<std::string> arg_database = {
        "database", available_dbs.c_str(), default_db_type
    };
    const command_line::arg_descriptor<bool> arg_blocks_dat = { "transactions.db", "Output in sqlite format", blocks_dat };

    command_line::add_arg(desc_cmd_sett, command_line::arg_data_dir, default_data_path.string());
    command_line::add_arg(desc_cmd_sett, command_line::arg_testnet_data_dir, default_testnet_data_path.string());
    command_line::add_arg(desc_cmd_sett, arg_output_file);
    command_line::add_arg(desc_cmd_sett, arg_testnet_on);
    command_line::add_arg(desc_cmd_sett, arg_log_level);
    command_line::add_arg(desc_cmd_sett, arg_database);
    command_line::add_arg(desc_cmd_sett, arg_block_stop);
    command_line::add_arg(desc_cmd_sett, arg_blocks_dat);

    command_line::add_arg(desc_cmd_only, command_line::arg_help);

    po::options_description desc_options("Allowed options");
    desc_options.add(desc_cmd_only).add(desc_cmd_sett);

    po::variables_map vm;
    bool r = command_line::handle_error_helper(desc_options, [&]() {
        po::store(po::parse_command_line(argc, argv, desc_options), vm);
        po::notify(vm);
        return true;
    });
    if (!r)
        return 1;

    if (command_line::get_arg(vm, command_line::arg_help)) {
        std::cout << "Monero '" << MONERO_RELEASE_NAME << "' (v" << MONERO_VERSION_FULL << ")" << ENDL << ENDL;
        std::cout << desc_options << std::endl;
        return 1;
    }

    log_level = command_line::get_arg(vm, arg_log_level);
    block_stop = command_line::get_arg(vm, arg_block_stop);

    log_space::get_set_log_detalisation_level(true, log_level);
    log_space::log_singletone::add_logger(LOGGER_CONSOLE, NULL, NULL);
/*    LOG_PRINT_L0("Starting...");
    LOG_PRINT_L0("Setting log level = " << log_level);*/

    bool opt_testnet = command_line::get_arg(vm, arg_testnet_on);
    bool opt_blocks_dat = command_line::get_arg(vm, arg_blocks_dat);

    std::string m_config_folder;

    auto data_dir_arg = opt_testnet ? command_line::arg_testnet_data_dir : command_line::arg_data_dir;
    m_config_folder = command_line::get_arg(vm, data_dir_arg);

    std::string db_type = command_line::get_arg(vm, arg_database);
    if (db_types_all.count(db_type) == 0) {
        std::cerr << "Invalid database type: " << db_type << std::endl;
        return 1;
    }
#if !defined(BERKELEY_DB)
    if (db_type == "berkeley") {
        LOG_ERROR("BerkeleyDB support disabled.");
        return false;
    }
#endif

    /* Open database */
    // CHANGE ME to path of database, open to see if we can open the database properly.
    if (command_line::has_arg(vm, arg_output_file))
        output_file_path = boost::filesystem::path(command_line::get_arg(vm, arg_output_file));
    else {
        fprintf(stderr, "No outputfile provided");
        exit(0);
    }

    rc = sqlite3_open(output_file_path.c_str(), &dbs);
    setupTable();
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(dbs));
        exit(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /*
    if (command_line::has_arg(vm, arg_output_file))
    output_file_path = boost::filesystem::path(command_line::get_arg(vm, arg_output_file));
    else
    output_file_path = boost::filesystem::path(m_config_folder) / "export" / "spri.db";
    LOG_PRINT_L0("Export output file: " << output_file_path.string());
    */

    // If we wanted to use the memory pool, we would set up a fake_core.

    // Use Blockchain instead of lower-level BlockchainDB for two reasons:
    // 1. Blockchain has the init() method for easy setup
    // 2. exporter needs to use get_current_blockchain_height(), get_block_id_by_height(), get_block_by_hash()
    //
    // cannot match blockchain_storage setup above with just one line,
    // e.g.
    //   Blockchain* blockchain_storage = new Blockchain(NULL);
    // because unlike blockchain_storage constructor, which takes a pointer to
    // tx_memory_pool, Blockchain's constructor takes tx_memory_pool object.
/*    LOG_PRINT_L0("Initializing source blockchain (BlockchainDB)");
*/    Blockchain* blockchain_storage = NULL;
    tx_memory_pool m_mempool(*blockchain_storage);
    blockchain_storage = new Blockchain(m_mempool);

    int db_flags = 0;

    BlockchainDB* db = nullptr;
    if (db_type == "lmdb") {
        db_flags |= MDB_RDONLY;
        db = new BlockchainLMDB();
    }
#if defined(BERKELEY_DB)
    else if (db_type == "berkeley")
        db = new BlockchainBDB();
#endif
    else {
        LOG_ERROR("Attempted to use non-existent database type: " << db_type);
        throw std::runtime_error("Attempting to use non-existent database type");
    }
/*    LOG_PRINT_L0("database: " << db_type);
*/
    boost::filesystem::path folder(m_config_folder);
    folder /= db->get_db_name();
    const std::string filename = folder.string();

/*    LOG_PRINT_L0("Loading blockchain from folder " << filename << " ...");
*/    try {
        db->open(filename, db_flags);
    }
    catch (const std::exception& e) {
/*        LOG_PRINT_L0("Error opening database: " << e.what());
*/        return 1;
    }
    r = blockchain_storage->init(db, opt_testnet);

    CHECK_AND_ASSERT_MES(r, false, "Failed to initialize source blockchain storage");
/*    LOG_PRINT_L0("Source blockchain storage initialized OK");
    LOG_PRINT_L0("Looping through blockchain....");*/

    r = parse_blockchain_pass1(blockchain_storage, NULL, output_file_path, block_stop);

    CHECK_AND_ASSERT_MES(r, false, "Failed to export blockchain raw data");
/*    LOG_PRINT_L0("Blockchain raw data exported OK");
*/
    // close the database
    sqlite3_close(dbs);
}
