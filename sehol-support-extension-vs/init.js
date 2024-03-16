// extension.js

const vscode = require('vscode');

function activate(context) {
    // Register the language
    vscode.languages.register({
        id: 'sehol',
        extensions: ['.sl'],
        aliases: ['Sehol', 'sl'],
    });

    // Set the configuration for the language
    vscode.languages.setLanguageConfiguration('Sehol', {
        indentationRules: {
            // Define indentation rules
        },
        // Add more language configuration if needed
    });

    // Register the language support
    context.subscriptions.push(
        vscode.languages.registerCompletionItemProvider('Sehol', {
            provideCompletionItems(document, position, token, context) {
                // Implement completion suggestions
            },
        })
    );

    // Implement more language features as needed

}
exports.activate = activate;
