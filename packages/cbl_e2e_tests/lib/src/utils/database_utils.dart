import 'package:cbl/cbl.dart';

import '../test_binding.dart';

/// Opens a test database and registers a tear down hook to clean it up when
/// the test scope is done.
///
/// The name of the database is generated by giving [dbName] to [testDbName]
/// unless [useNameDirectly] is `true`. Then [dbName] is used directly.
///
/// The database will be created in the [tmpDir].
Future<Database> openTestDb(
  String dbName, {
  bool useNameDirectly = false,
  bool autoClose = true,
}) async {
  final db = await Database.open(
    useNameDirectly ? dbName : testDbName(dbName),
    config: DatabaseConfiguration(
      directory: tmpDir,
    ),
  );

  if (autoClose) addTearDown(db.close);

  return db;
}

extension DatabaseUtilsExtension on Database {
  /// Returns a stream wich emits the ids of all the documents in this database.
  Stream<String> getAllIds() async* {
    final resultSet =
        await query('SELECT META.id AS id').then((query) => query.execute());

    for (final result in resultSet.asDicts) {
      yield result['id'].asString;
    }
  }

  /// Returns a stream which emits the ids of all the documents in the
  /// database when they change.
  Stream<List<String>> watchAllIds() => query('SELECT META.id AS id')
      .asStream()
      .asyncExpand((q) => q.changes())
      .map((resultSet) => resultSet.map((rs) => rs['id'].asString).toList());

  /// Deletes all documents in this database and returns whether any documents
  /// where deleted.
  Future<bool> deleteAllDocuments() async {
    var deletedAnyDocument = false;
    await for (final id in getAllIds()) {
      final doc = await getDocument(id);
      await doc?.delete();
      deletedAnyDocument = doc != null;
    }
    return deletedAnyDocument;
  }
}