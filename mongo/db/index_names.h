
#pragma once

#include <string>

namespace mongo {

    class BSONObj;

    /**
     * We need to know what 'type' an index is in order to plan correctly.  We can't entirely rely
     * on the key pattern to tell us what kind of index we have.
     *
     * An example of the Bad Thing That We Must Avoid:
     * 1. Create a 2dsphere index in 2.4, insert some docs.
     * 2. Downgrade to 2.2.  Insert some more docs into the collection w/the 2dsphere
     *    index.  2.2 treats the index as a normal btree index and creates keys accordingly.
     * 3. Using the 2dsphere index in 2.4 gives wrong results or assert-fails or crashes as
     *    the data isn't what we expect.
     */
    enum IndexType {
        INDEX_BTREE,
        INDEX_2D,
        INDEX_HAYSTACK,
        INDEX_2DSPHERE,
        INDEX_TEXT,
        INDEX_HASHED,
		INDEX_TEST,
    };

    /**
     * We use the std::string representation of index names all over the place, so we declare them all
     * once here.
     */
    class IndexNames {
    public:
        static const std::string GEO_2D;
        static const std::string GEO_HAYSTACK;
        static const std::string GEO_2DSPHERE;
        static const std::string TEXT;
        static const std::string HASHED;
        static const std::string BTREE;
		static const std::string TEST;

        /**
         * True if is a regular (non-plugin) index or uses a plugin that existed before 2.4.
         * These plugins are grandfathered in and allowed to exist in DBs where
         * DataFileVersion::is24IndexClean() returns false.
         */
        static bool existedBefore24(const std::string& name);

        /**
         * Return the first std::string value in the provided object.  For an index key pattern,
         * a field with a non-string value indicates a "special" (not straight Btree) index.
         */
        static std::string findPluginName(const BSONObj& keyPattern);

        /**
         * Is the provided access method name one we recognize?
         */
        static bool isKnownName(const std::string& name);

        /**
         * Convert an index name to an IndexType.
         */
        static IndexType nameToType(const std::string& accessMethod);
    };

}  // namespace mongo
