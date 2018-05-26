//
//  CoreData.swift
//  IZA
//
//  Created by Tomas Lapsansky on 21/05/2018.
//  Copyright © 2018 Tomas Lapsansky. All rights reserved.
//

import UIKit
import CoreData

// Functions with CoreData

func loadCoreData() -> Array<NSManagedObject>{
    
    let context = returnContext()
    
    let request = NSFetchRequest<NSFetchRequestResult>(entityName: "Goals")
    
    request.returnsObjectsAsFaults = false
    
    var items = [NSManagedObject]()
    
    // Loading from CoreData
    do
    {
        let results = try context.fetch(request)
        
        if results.count > 0 {
            for result in results as! [NSManagedObject] {
                items.append(result)
            }
        }
    }
    catch
    {
        // HANDLER
    }
    
    return items
}

func insertCoreData(title: String) -> NSManagedObject {
    
    let context = returnContext()
    
    let newTitle = NSEntityDescription.insertNewObject(forEntityName: "Goals", into: context)
    newTitle.setValue(title, forKey: "title")
    newTitle.setValue("", forKey: "desc")
    newTitle.setValue(false, forKey: "complete")
    newTitle.setValue(false, forKey: "alert")
    newTitle.setValue(Date(), forKey: "start_date")
    newTitle.setValue(Date(), forKey: "alert_date")
    
    do
    {
        try context.save()
    }
    catch
    {
        // HANDLER
    }
    
    return newTitle
}

func deleteCoreData(goal: NSManagedObject) {
    
    let context = returnContext()
    
    context.delete(goal)
    
    do
    {
        try context.save()
    }
    catch
    {
        // HANDLER
    }
    
}

func returnContext() -> NSManagedObjectContext {
    
    let appDelegate = UIApplication.shared.delegate as! AppDelegate
    
    return appDelegate.persistentContainer.viewContext
}
