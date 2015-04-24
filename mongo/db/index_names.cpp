
#include "mongo/db/index_names.h"

#include "mongo/db/jsobj.h"

namespace mongo {

    using std::string;

    const string IndexNames::GEO_2D = "2d";
    const string IndexNames::GEO_HAYSTACK = "geoHaystack";
    const string IndexNames::GEO_2DSPHERE = "2dsphere";
    const string IndexNames::TEXT = "text";
    const string IndexNames::HASHED = "hashed";
    const string IndexNames::BTREE = "";
	const string IndexNames::TEST = "test";

    // static
    string IndexNames::findPluginName(const BSONObj& keyPattern) {
        BSONObjIterator i(keyPattern);

        while (i.more()) {
            BSONElement e = i.next();
            if (String != e.type()) { continue; }
            return e.String();
        }

        return IndexNames::BTREE;
    }

    // static
    bool IndexNames::existedBefore24(const string& name) {
        return name == IndexNames::BTREE
            || name == IndexNames::GEO_2D
            || name == IndexNames::GEO_HAYSTACK
            || name == IndexNames::HASHED;
    }

    // static
    bool IndexNames::isKnownName(const string& name) {
        return    name == IndexNames::GEO_2D
               || name == IndexNames::GEO_2DSPHERE
               || name == IndexNames::GEO_HAYSTACK
               || name == IndexNames::TEXT
               || name == IndexNames::HASHED
               || name == IndexNames::BTREE
			   || name == IndexNames::TEST;
    }

    // static
    IndexType IndexNames::nameToType(const string& accessMethod) {
        if (IndexNames::GEO_2D == accessMethod) {
            return INDEX_2D;
        }
        else if (IndexNames::GEO_HAYSTACK == accessMethod) {
            return INDEX_HAYSTACK;
        }
        else if (IndexNames::GEO_2DSPHERE == accessMethod) {
            return INDEX_2DSPHERE;
        }
        else if (IndexNames::TEXT == accessMethod) {
            return INDEX_TEXT;
        }
        else if (IndexNames::HASHED == accessMethod) {
            return INDEX_HASHED;
        }
		else if (IndexNames::TEST == accessMethod) {
			return INDEX_TEST;
		}
        else {
            return INDEX_BTREE;
        }
    }

}  // namespace mongo
