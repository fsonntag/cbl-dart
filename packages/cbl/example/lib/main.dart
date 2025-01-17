// ignore_for_file: avoid_print

import 'package:cbl/cbl.dart';

/// You need to specify how to access the native libraries, on top of which
/// `cbl` is implemented.
LibrariesConfiguration getLibraries() {
  throw UnimplementedError('TODO');
}

Future<void> main() async {
  // Couchbase Lite needs to be initialized before it can be used.
  CouchbaseLite.init(libraries: getLibraries());

  final db = await Database.openAsync('chat-messages');

  final doc = MutableDocument({
    'type': 'message',
    'body': 'Heyo',
    'from': 'Alice',
  });

  await db.saveDocument(doc);

  await db.close();
}
