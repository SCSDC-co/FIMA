/*
 * src/commands/trash/trash.cpp
 * include/commands/trash/trash.h
 *
 * The implementation of the `trash` subcommand
 *
 * Copyright (C) 2026 Giuliano De Amicis. All rights reserved.
 * This software is licensed under the GPL-3.0-or-later.
 * See LICENSE file for details.
 */

#include "commands/trash/trash.h"

#include <CLI/App.hpp>
#include <vector>

#include "commands/trash/empty.h"
#include "commands/trash/list.h"
#include "commands/trash/remove.h"
#include "commands/trash/restore.h"

namespace fima {

namespace commands {

void
setup_trash(CLI::App& app, bool& yes, std::vector<std::string>& ids, bool& list_plain)
{
    CLI::App* subcmd = app.add_subcommand("trash", "Trash operations")
                         ->configurable(false)
                         ->require_subcommand(1, 0);

    CLI::App* trash_list =
      subcmd->add_subcommand("list", "List all the files in the trash")->configurable(false);

    trash_list->add_flag("-p,--plain", list_plain, "Plain output")->configurable(true);

    trash_list->usage("fima trash list");

    trash_list->callback([&]() { fima::commands::trash::list(list_plain); });

    CLI::App* trash_empty =
      subcmd->add_subcommand("empty", "Empties the trash")->configurable(false);

    trash_empty->add_flag("-y,--yes", yes, "Doesn't ask for the confirmation")
      ->multi_option_policy(CLI::MultiOptionPolicy::Throw)
      ->configurable(true);

    trash_empty->usage("fima trash empty [OPTIONS]");

    trash_empty->callback([&]() { fima::commands::trash::empty(yes); });

    CLI::App* trash_restore =
      subcmd->add_subcommand("restore", "Restore an item from the trash")->configurable(false);

    trash_restore->add_option("id", ids, "The ID of the item to restore");

    trash_restore->usage("fima trash restore ID");

    trash_restore->callback([&]() { fima::commands::trash::restore(ids); });

    CLI::App* trash_remove =
      subcmd->add_subcommand("remove", "Remove an item from the trash")->configurable(false);

    trash_remove->add_option("id", ids, "The ID of the item to remove");

    trash_remove->usage("fima trash remove ID");

    trash_remove->callback([&]() { fima::commands::trash::remove(ids); });
}

} // namespace commands

} // namespace fima
