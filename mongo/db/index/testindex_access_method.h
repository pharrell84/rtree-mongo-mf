
#pragma once

#include <boost/scoped_ptr.hpp>
#include <vector>

#include "mongo/base/disallow_copying.h"
#include "mongo/db/index/index_access_method.h"
#include "mongo/db/index/index_cursor.h"
#include "mongo/db/index/index_descriptor.h"
#include "mongo/db/jsobj.h"
#include "mongo/db/record_id.h"
#include "mongo/db/storage/sorted_data_interface.h"

namespace mongo {

	class ExternalSortComparison;

	class TestIndexAccessMethod : public IndexAccessMethod {
		//MONGO_DISALLOW_COPYING(TestIndexAccessMethod);
	public:
		TestIndexAccessMethod(IndexCatalogEntry* btreeState,
			SortedDataInterface* btree);

		virtual ~TestIndexAccessMethod() { }

		virtual Status insert(OperationContext* txn,
			const BSONObj& obj,
			const RecordId& loc,
			const InsertDeleteOptions& options,
			int64_t* numInserted);

		virtual Status remove(OperationContext* txn,
			const BSONObj& obj,
			const RecordId& loc,
			const InsertDeleteOptions& options,
			int64_t* numDeleted);

		virtual Status validateUpdate(OperationContext* txn,
			const BSONObj& from,
			const BSONObj& to,
			const RecordId& loc,
			const InsertDeleteOptions& options,
			UpdateTicket* ticket);

		virtual Status update(OperationContext* txn,
			const UpdateTicket& ticket,
			int64_t* numUpdated);

		virtual Status newCursor(OperationContext* txn,
			const CursorOptions& opts,
			IndexCursor** out) const;

		virtual Status initializeAsEmpty(OperationContext* txn);

		virtual Status touch(OperationContext* txn, const BSONObj& obj);

		virtual Status touch(OperationContext* txn) const;

		virtual Status validate(OperationContext* txn, bool full, int64_t* numKeys,
			BSONObjBuilder* output);

		virtual bool appendCustomStats(OperationContext* txn, BSONObjBuilder* output, double scale)
			const;
		virtual long long getSpaceUsedBytes(OperationContext* txn) const;

		// XXX: consider migrating callers to use IndexCursor instead
		virtual RecordId findSingle(OperationContext* txn, const BSONObj& key) const;

		virtual IndexAccessMethod* initiateBulk(OperationContext* txn);

		virtual Status commitBulk(IndexAccessMethod* bulk,
			bool mayInterrupt,
			bool dupsAllowed,
			std::set<RecordId>* dups);

	protected:
		// See below for body.
		class TestIndexPrivateUpdateData;

		// Determines whether it's OK to ignore ErrorCodes::KeyTooLong for this OperationContext
		bool ignoreKeyTooLong(OperationContext* txn);

		IndexCatalogEntry* _btreeState; // owned by IndexCatalogEntry
		const IndexDescriptor* _descriptor;


	private:
		virtual void getKeys(const BSONObj& obj, BSONObjSet* keys) const;

		void removeOneKey(OperationContext* txn,
			const BSONObj& key,
			const RecordId& loc,
			bool dupsAllowed);

		boost::scoped_ptr<SortedDataInterface> _newInterface; // sorteddatainterface is a virtual class in storage subfolder

		//boost::scoped_ptr<BtreeKeyGenerator> _keyGenerator;

	};

	
	// * What data do we need to perform an update?
	
	class TestIndexAccessMethod::TestIndexPrivateUpdateData
		: public UpdateTicket::PrivateUpdateData{
	public:
		virtual ~TestIndexPrivateUpdateData() { }

		BSONObjSet oldKeys, newKeys;

		// These point into the sets oldKeys and newKeys.
		std::vector<BSONObj*> removed, added;

		RecordId loc;
		bool dupsAllowed;
	};
	
}  // namespace mongo
