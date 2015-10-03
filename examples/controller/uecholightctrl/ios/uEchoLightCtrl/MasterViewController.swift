/******************************************************************
*
* uEcho for Swift
*
* Copyright (C) The uecho Authors 2015
*
* This is licensed under BSD-style license, see file COPYING.
*
******************************************************************/

import UIKit
import uEcho

class MasterViewController: UITableViewController {

  var detailViewController: DetailViewController? = nil
  var objects = [uEchoObject]()
  var controller: uEchoLightController! = nil

  override func viewDidLoad() {
    super.viewDidLoad()
    
    let reloadButton = UIBarButtonItem(barButtonSystemItem: .Add, target: self, action: "searchObjects:")
    self.navigationItem.rightBarButtonItem = reloadButton
    if let split = self.splitViewController {
        let controllers = split.viewControllers
        self.detailViewController = (controllers[controllers.count-1] as! UINavigationController).topViewController as? DetailViewController
    }
    
    self.controller = uEchoLightController()
    self.controller.listner = self.controllerMessageReceived
    self.controller.start();
  }

  override func viewWillAppear(animated: Bool) {
    self.clearsSelectionOnViewWillAppear = self.splitViewController!.collapsed
    super.viewWillAppear(animated)
  }

  override func viewDidAppear(animated: Bool) {
    self.controller.search();
    super.viewDidAppear(animated)
  }
  override func didReceiveMemoryWarning() {
    super.didReceiveMemoryWarning()
  }

  func searchObjects(sender: AnyObject) {
    UIApplication.sharedApplication().networkActivityIndicatorVisible = true
    self.controller.search();
  }

  // MARK: - Segues

  override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
    if segue.identifier == "showDetail" {
        if let indexPath = self.tableView.indexPathForSelectedRow {
            let object = objects[indexPath.row]
            let controller = (segue.destinationViewController as! UINavigationController).topViewController as! DetailViewController
            controller.detailItem = object
            controller.navigationItem.leftBarButtonItem = self.splitViewController?.displayModeButtonItem()
            controller.navigationItem.leftItemsSupplementBackButton = true
        }
    }
  }

  // MARK: - Table View

  override func numberOfSectionsInTableView(tableView: UITableView) -> Int {
    return 1
  }

  override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
    return objects.count
  }

  override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
    let cell = tableView.dequeueReusableCellWithIdentifier("Cell", forIndexPath: indexPath)

    let object = objects[indexPath.row]
    cell.textLabel!.text = object.code.description
    return cell
  }

  override func tableView(tableView: UITableView, canEditRowAtIndexPath indexPath: NSIndexPath) -> Bool {
    return false
  }

  // MARK: - uEcho Listener
  
  func controllerMessageReceived(msg : uEchoMessage) {
    UIApplication.sharedApplication().networkActivityIndicatorVisible = false
    self.objects = self.controller.objects
    self.tableView.reloadData();
  }

}

