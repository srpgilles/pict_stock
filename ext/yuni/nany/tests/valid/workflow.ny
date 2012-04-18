# Test containing a workflow

public workflow DocumentWorkflow
{
states
	default intro;
	#! Introduction
	state approved;
	state archived;
	state created;
	state deleted;
	state updated;
	state waitingForApproval;
	state rejected;

transitions
	default forbid;
	allow waitingForApproval => approved;
	allow created => updated;
	allow approved, updated, rejected, deleted => updated;
	allow updated => waitingForApproval;
	allow approved => archived;
	allow *, -archived => rejected, deleted;
}


public class Document
{
published
   #! A read/write property, which is a workflow
   abstract property status: DocumentWorkflow;
   #! Title of the document
   abstract property title;
   #! Author of the document
   abstract property author;
   #! Summary
   abstract property summary;
   #! Content
   abstract property content;
   #! Date of creation
   property creationDate read pCreationDate;
   #! Timestamp of the last update
   property lastUpdate read pLastUpdate;

protected
   #! Timestamp of the last update
   var pLastUpdate: timestamp;
   #! Date of creation
   var pCreationDate: timestamp;
}

