#pragma once

#include "Debug.hpp"

/**
 * @class Message
 * @brief Base class for messages used in a messaging system.
 *
 * This class provides a mechanism to identify and handle different types of messages.
 */
class Message;

/**
 * @brief Concept to check if a type is derived from Message.
 * @tparam T The type to check.
 */
template <typename T>
concept IsMessageType = std::is_base_of<Message, T>::value;

/**
 * @class Message
 * @brief Base class for messages used in a messaging system.
 *
 * This class provides a mechanism to identify and handle different types of messages.
 */
class Message
{
protected:
    /**
     * @brief Protected constructor for initializing the message type.
     * @param typeId_ The unique identifier for the message type.
     */
    Message(const size_t typeId_) : typeId(typeId_) {}

public:
    /**
     * @brief Virtual destructor to allow proper destruction of derived classes.
     */
    virtual ~Message() = default;

    const size_t typeId; /**< Unique identifier for the message type. */

    /**
     * @brief Checks if the message is of a specific type.
     * @tparam T The type to check.
     * @return True if the message is of type T, false otherwise.
     *
     * This function is part of the message type identification mechanism.
     */
    template <typename T>
        requires IsMessageType<T>
    bool IsType() const
    {
        return typeId == T::Type;
    }

    /**
     * @brief Attempts to cast the message to a specific type.
     * @tparam T The type to cast to.
     * @return A pointer to the casted type if successful, nullptr otherwise.
     *
     * This function is part of the message type casting mechanism.
     */
    template <typename T>
        requires IsMessageType<T>
    T* TryCast() const
    {
        if (IsType<T>())
        {
            return static_cast<T*>(const_cast<Message*>(this));
        }
        else
        {
            return nullptr;
        }
    }

    /**
     * @brief Placeholder for handling messages.
     */
    void CallHandler()
    {

    }
};

/**
 * @class MessageType
 * @brief Template class for defining message types.
 * @tparam T The derived message type.
 *
 * This class inherits from Message and provides a unique identifier for the message type.
 */
template <typename T>
struct MessageType : public Message
{
public:
    constexpr static size_t Type = TypeInfo::ID<T>; /**< Unique identifier for the message type. */

protected:
    /**
     * @brief Protected constructor to set the message type during initialization.
     */
    MessageType() : Message(Type) {}
};

/**
 * @interface IMessageHandler
 * @brief Interface for handling messages.
 */
struct IMessageHandler
{
    inline static IMessageHandler* allObjects[200]; /**< Static vector to store pointers to trackable objects. */

    /**
     * @brief Handles messages.
     * @param message The message to handle.
     */
    virtual void HandleMessages(Message& message) {}

    virtual void HandleMessages(const Message& message)
    {
        HandleMessages(const_cast<Message&>(message));
    }

	inline static void Init()
	{
		for (int i = 0; i < 200; i++)
		{
			allObjects[i] = nullptr;
		}
	}

    /**
     * @brief Constructor for TrackableObject.
     *
     * Registers the object in the objects vector upon creation.
     */
    IMessageHandler()
    {
		for (int i = 0; i < 200; i++)
		{
			if (allObjects[i] == nullptr)
			{
				allObjects[i] = this;
				return;
			}
		}
    }

    /**
     * @brief Virtual destructor for TrackableObject.
     *
     * Unregisters the object from the objects vector upon destruction.
     */
    virtual ~IMessageHandler()
    {
		for (int i = 0; i < 200; i++)
		{
			if (allObjects[i] == this)
			{
				allObjects[i] = nullptr;
			}
		}
    }

	inline static void DeleteAll()
	{
		for (int i = 0; i < 200; i++)
		{
			if (allObjects[i] != nullptr)
			{
				delete allObjects[i];
				allObjects[i] = nullptr;
			}
		}
	}
};

/**
 * @struct MessageDispatcher
 * @brief Dispatches messages to the appropriate handlers.
 */
struct MessageDispatcher
{
    /**
     * @brief Handles a specific type of message.
     * @tparam M The type of message.
     * @param handler The message handler.
     * @param message The message to handle.
     */
    template <typename M>
    static void HandleWrapper(auto& handler, Message& message)
    {
        if (M* msg = message.TryCast<M>())
        {
            handler.HandleMessage(*msg);
        }
    }

public:

    /**
     * @brief Dispatches a set of messages to the appropriate handlers.
     * @tparam Ts The types of messages.
     * @param handler The message handler.
     * @param message The message to handle.
     */
    template <typename... Ts>
    static void DispatchMessages(auto& handler, Message& message)
    {
        (HandleWrapper<Ts>(handler, message), ...);
    }
};

/**
 * @class MessageHandler
 * @brief Handles a set of messages.
 * @tparam T The type of the handler.
 * @tparam Messages The types of messages to handle.
 */
template <typename T, typename... Messages>
struct MessageHandler : public IMessageHandler
{
    /**
     * @brief Handles messages.
     * @param message The message to handle.
     */
    void HandleMessages(Message& message) override
    {
        MessageDispatcher::DispatchMessages<Messages...>(*(T*)this, message);
    }
};

/**
 * @class OverideMessageHandler
 * @brief Overrides message handling with a set of messages.
 * @tparam T The type of the handler.
 * @tparam Overide The overridden handler.
 * @tparam Messages The types of messages to handle.
 */
template <typename T, typename Overide, typename... Messages>
struct OverideMessageHandler : public Overide
{
    /**
     * @brief Handles messages.
     * @param message The message to handle.
     */
    void HandleMessages(Message& message) override
    {
        MessageDispatcher::DispatchMessages<Messages...>(*(T*)this, message);
    }
};

/**
 * @class ExtendedMessageHandler
 * @brief Extends message handling with a set of messages.
 * @tparam T The type of the handler.
 * @tparam Extension The extended handler.
 * @tparam Messages The types of messages to handle.
 */
template <typename T, typename Extension, typename... Messages>
struct ExtendedMessageHandler : public Extension
{
    /**
     * @brief Handles messages.
     * @param message The message to handle.
     */
    void HandleMessages(Message& message) override
    {
        Extension::HandleMessages(message);
        MessageDispatcher::DispatchMessages<Messages...>(*(T*)this, message);
    }
};


/**** Examples ****/

// struct ExampleHandlerA : public MessageHandler<ExampleHandlerA, MessageA, MessageB, MessageC>
// {
// 	virtual void HandleMessage(MessageA&)
// 	{
// 		jo_printf(0, line++, "ExampleHandlerA MessageA");
// 	}

// 	virtual void HandleMessage(MessageB&)
// 	{
// 		jo_printf(0, line++, "ExampleHandlerA MessageB");
// 	}

// 	virtual void HandleMessage(MessageC&)
// 	{
// 		jo_printf(0, line++, "ExampleHandlerA MessageC");
// 	}
// };

// struct ExampleHandlerB : public ExampleHandlerA
// {
// 	void HandleMessage(MessageC& message) override
// 	{
// 		jo_printf(0, line++, "ExampleHandlerB MessageC");
// 	}
// };

// struct ExampleHandlerC : public ExtendedMessageHandler<ExampleHandlerC, ExampleHandlerB, MessageD>
// {
// 	void HandleMessage(MessageD& message)
// 	{
// 		jo_printf(0, line++, "ExampleHandlerC MessageD");
// 	}
// };


// struct ExampleHandlerA : public IMessageHandler
// {
// 	void HandleMessages(Message& message) override
// 	{
// 		switch (message.typeId)
// 		{
// 		case MessageA::Type:
// 			jo_printf(0, line++, "ExampleHandlerA MessageA");
// 			break;
// 		case MessageB::Type:
// 			jo_printf(0, line++, "ExampleHandlerA MessageB");
// 			break;
// 		case MessageC::Type:
// 			jo_printf(0, line++, "ExampleHandlerA MessageC");
// 			break;
// 		default:
// 			jo_printf(0, line++, "ExampleHandlerA Unknown Message");
// 			break;
// 		}
// 	}
// };

// struct ExampleHandlerB : public ExampleHandlerA
// {
// 	void HandleMessages(Message& message) override
// 	{
// 		ExampleHandlerA::HandleMessages(message);
// 		switch (message.typeId)
// 		{
// 		case MessageC::Type:
// 			jo_printf(0, line++, "ExampleHandlerB MessageC");
// 			break;
// 		default:
// 			jo_printf(0, line++, "ExampleHandlerB Unknown Message");
// 			break;
// 		}
// 	}
// };

// struct ExampleHandlerC : ExampleHandlerB
// {
// 	void HandleMessages(Message& message) override
// 	{
// 		ExampleHandlerB::HandleMessages(message);
// 		switch (message.typeId)
// 		{
// 		case MessageD::Type:
// 			jo_printf(0, line++, "ExampleHandlerC MessageD");
// 			break;
// 		default:
// 			jo_printf(0, line++, "ExampleHandlerC Unknown Message");
// 			break;
// 		}
// 	}
// };

// void Example()
// {
// 	ExampleHandlerA handlerA;
// 	ExampleHandlerB handlerB;
// 	ExampleHandlerC handlerC;

// 	MessageA messageA;
// 	MessageB messageB;
// 	MessageC messageC;

// 	handlerA.HandleMessages(messageA);
// 	handlerA.HandleMessages(messageB);
// 	handlerA.HandleMessages(messageC);
// 	handlerA.HandleMessages(MessageD());
// 	jo_printf(0, line++, "");

// 	handlerB.HandleMessages(messageA);
// 	handlerB.HandleMessages(messageB);
// 	handlerB.HandleMessages(messageC);
// 	handlerB.HandleMessages(MessageD());

// 	jo_printf(0, line++, "");
// 	handlerC.HandleMessages(messageA);
// 	handlerC.HandleMessages(messageB);
// 	handlerC.HandleMessages(messageC);
// 	handlerC.HandleMessages(MessageD());
// }
